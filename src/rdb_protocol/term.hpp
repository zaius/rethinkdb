#ifndef RDB_PROTOCOL_TERM_HPP_
#define RDB_PROTOCOL_TERM_HPP_
#include <string>

#include "utils.hpp"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include "containers/scoped.hpp"
#include "rdb_protocol/datum.hpp"
#include "rdb_protocol/ql2.pb.h"

namespace ql {
class env_t;
class val_t;
class term_t {
public:
    term_t(env_t *_env);
    virtual ~term_t();

    virtual const char *name() const = 0;
    val_t *eval(bool use_cached_val = true);

    val_t *new_val(datum_t *d);
    val_t *new_val() { return new_val(new datum_t()); }
    template<class T>
    val_t *new_val(T t) { return new_val(new datum_t(t)); }
    template<class T>
    void set_bt(T t) { frame.init(new backtrace_t::frame_t(t)); }
    bool has_bt() { return frame.has(); }
    backtrace_t::frame_t get_bt() { return *frame.get(); }
private:
    virtual val_t *eval_impl() = 0;
    val_t *cached_val;
    env_t *env;

    scoped_ptr_t<backtrace_t::frame_t> frame;
};
term_t *compile_term(env_t *env, const Term2 *t);

}
#endif // RDB_PROTOCOL_TERM_HPP_