#ifndef TGFUNC_HPP
#define TGFUNC_HPP
#if __cplusplus < 202302
#include "compat/tgfunc.h++"
#else
#include <chrono>
#include <functional>
#include <print>
#include <source_location>
#include <string>
#define TGFUNC_DBG std::source_location::current()
namespace tgfunc
{
    using namespace std::chrono;
    using desc_t    = std::string;
    using time_t    = nanoseconds;
    using clock_t   = high_resolution_clock;
    using src_loc_t = std::source_location;
    using ullong    = unsigned long long;
    // inline namespace cls
    // {
    template <class ret, class ...args>
    struct wrp
    {
        using func_t  = std::function<ret (args...)>;
        using _func_t = std::function<void ()>;
        func_t body;
        _func_t cstr, dstr;
        desc_t desc;
        wrp
        (
            const func_t body,
            const _func_t cstr = [] {},
            const _func_t dstr = [] {},
            const desc_t desc  = "wrp"
        ) : body(body), cstr(cstr), dstr(dstr), desc(desc) {}
        ret operator()(const args ..._args) const
        {
            cstr();
            const ret _ret = body(_args...);
            dstr();
            return _ret;
        }
    };
    template <class ...args>
    struct wrp<void, args...>
    {
        using func_t  = std::function<void (args...)>;
        using _func_t = std::function<void ()>;
        func_t body;
        _func_t cstr, dstr;
        desc_t desc;
        wrp
        (
            const func_t body,
            const _func_t cstr = [] {},
            const _func_t dstr = [] {},
            const desc_t desc  = "wrp"
        ) : body(body), cstr(cstr), dstr(dstr), desc(desc) {}
        void operator()(const args ..._args) const
        {
            cstr();
            body(_args...);
            dstr();
        }
    };
    template <class ret, class ...args>
    struct tmr
    {
        using func_t = std::function<ret (args...)>;
        func_t body;
        desc_t desc;
        struct ret_time
        {
            ret _ret;
            time_t _time;
            ret_time
            (
                const ret _ret,
                const time_t _time
            ) : _ret(_ret), _time(_time) {}
        };
        tmr
        (
            const func_t body,
            const desc_t desc = "tmr"
        ) : body(body), desc(desc) {}
        ret_time operator()(const args ..._args) const
        {
            const time_point<clock_t> begin = clock_t::now();
            const ret _ret                  = body(_args...);
            const time_t time               =
                clock_t::now() - begin;
            return { _ret, time };
        }
    };
    template <class ...args>
    struct tmr<void, args...>
    {
        using func_t = std::function<void (args...)>;
        func_t body;
        desc_t desc;
        tmr
        (
            const func_t body,
            const desc_t desc = "tmr"
        ) : body(body), desc(desc) {}
        time_t operator()(const args ..._args) const
        {
            const time_point<clock_t> begin = clock_t::now();
            body(_args...);
            return clock_t::now() - begin;
        }
    };
    template <class ret, class ...args>
    struct dbg
    {
        using func_t = std::function<ret (args...)>;
        func_t body;
        src_loc_t src_loc;
        desc_t desc;
        dbg
        (
            const func_t body,
            const src_loc_t src_loc,
            const desc_t desc = "dbg"
        ) : body(body), src_loc(src_loc), desc(desc) {}
        ret operator()(const args ..._args) const
        {
            std::println("File: {}.", src_loc.file_name());
            std::println("Line: {}.", src_loc.line());
            std::println("Column: {}.", src_loc.column());
            std::println
            (
                "Function: {}.",
                src_loc.function_name()
            );
            std::println("Beginning of {}.", desc);
            const ret _ret = body(_args...);
            std::println("End of {}.", desc);
            return _ret;
        }
    };
    template <class ...args>
    struct dbg<void, args...>
    {
        using func_t = std::function<void (args...)>;
        func_t body;
        src_loc_t src_loc;
        desc_t desc;
        dbg
        (
            const func_t body,
            const src_loc_t src_loc,
            const desc_t desc = "dbg"
        ) : body(body), src_loc(src_loc), desc(desc) {}
        void operator()(const args ..._args) const
        {
            std::println
            (
                "File:     {}.",
                src_loc.file_name()
            );
            std::println
            (
                "Line:     {}.",
                src_loc.line()
            );
            std::println
            (
                "Column:   {}.",
                src_loc.column()
            );
            std::println
            (
                "Function: {}.",
                src_loc.function_name()
            );
            std::println("Beginning of {}.", desc);
            body(_args...);
            std::println("End of       {}.", desc);
        }
    };
    // }
    // inline namespace func
    // {
    template <class type>
    type if_else
    (
        const bool cond,
        const std::function<type ()> func_if_true,
        const std::function<type ()> func_if_false =
            [] { return type(); }
    )
    {
        return cond ? func_if_true() : func_if_false();
    }
    // }
}
#endif
#endif
