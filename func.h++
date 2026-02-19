#ifndef FUNC_HPP
#define FUNC_HPP
#if __cplusplus < 202302
#include "compat/func.h++"
#else
#include <atomic>
#include <chrono>
#include <functional>
#include <print>
#include <source_location>
#include <string>
#define FUNC_DBG std::source_location::current()
namespace func
{
    using namespace std::chrono;
    using func_t    = std::function<void ()>;
    using desc_t    = std::string;
    using time_t    = nanoseconds;
    using clock_t   = high_resolution_clock;
    using src_loc_t = std::source_location;
    using ullong    = unsigned long long;
    // inline namespace cls
    // {
    struct wrp
    {
        func_t body, cstr, dstr;
        desc_t desc;
        wrp
        (
            const func_t body,
            const func_t cstr = [] {},
            const func_t dstr = [] {},
            const desc_t desc = "wrp"
        ) : body(body), cstr(cstr), dstr(dstr), desc(desc) {}
        void operator()() const
        {
            cstr();
            body();
            dstr();
        }
    };
    struct tmr
    {
        func_t body;
        desc_t desc;
        tmr
        (
            const func_t body,
            const desc_t desc = "tmr"
        ) : body(body), desc(desc) {}
        time_t operator()() const
        {
            const time_point<clock_t> begin = clock_t::now();
            body();
            return clock_t::now() - begin;
        }
    };
    struct dbg
    {
        func_t body;
        src_loc_t src_loc;
        desc_t desc;
        dbg
        (
            const func_t body,
            const src_loc_t src_loc,
            const desc_t desc = "dbg"
        ) : body(body), src_loc(src_loc), desc(desc) {}
        void operator()() const
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
            body();
            std::println("End of {}.", desc);
        }
    };
    // }
    // inline namespace func
    // {
    void if_else
    (   
        const bool cond,
        const func_t func_if_true,
        const func_t func_if_false = [] {}
    )
    {
        cond ? func_if_true() : func_if_false();
    }
    void loop
    (
        const ullong i,
        const func_t func = [] {}
    )
    {
        for (std::atomic_ullong j(0); j < i; j++)
            func();
    }
    void loop
    (
        const ullong i,
        const std::function<void (const ullong)> func
    )
    {
        for (std::atomic_ullong j(0); j < i; j++)
            func(j);
    }
    [[noreturn]] void inf_loop(const func_t func = [] {})
    {
        for (;;)
            func();
    }
    // }
}
#endif
#endif
