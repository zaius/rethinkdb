#!/usr/bin/env python
import sys, os, time
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir, 'common')))
import driver, http_admin
from workload_common import MemcacheConnection


with driver.Metacluster() as metacluster:
    cluster = driver.Cluster(metacluster)
    print "Starting cluster..."
    processes = [driver.Process(cluster, driver.Files(metacluster))
        for i in xrange(2)]
    for process in processes:
        process.wait_until_started_up()
    print "Creating namespace..."
    http = http_admin.ClusterAccess([("localhost", p.http_port) for p in processes])
    dc = http.add_datacenter()
    for machine_id in http.machines:
        http.move_server_to_datacenter(machine_id, dc)
    ns = http.add_namespace(protocol = "memcached", primary = dc)
    time.sleep(10)
    host, port = driver.get_namespace_host(ns.port, processes)

    distribution = http.get_distribution(ns)

    with MemcacheConnection(host, port) as mc:
        for i in range(10000):
            mc.set(str(i) * 10, str(i)*20)

    time.sleep(1)

    distribution = http.get_distribution(ns)

    cluster.check_and_stop()