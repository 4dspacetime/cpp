#ifndef TERM_HPP
#define TERM_HPP
#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
namespace term
{
    using desc_t  = std::string;
    using ldouble = long double;
    // inline namespace func
    // {
    void clear()
    {
#ifdef _WIN32
        system("cls");
#elif
        system("clear");
#endif
    }
    void sleep(ldouble s)
    {
        std::this_thread::sleep_for
        (
            std::chrono::duration<ldouble>(s)
        );
    }
    // }
    // inline namespace cls
    // {
    struct cli
    {
        std::string cmmd;
        std::vector<std::string> args;
        desc_t desc;
        cli
        (
            const std::string cmmd,
            const std::vector<std::string> args = {},
            const std::string desc              = "cli"
        ) : cmmd(cmmd), args(args), desc(desc) {}
        void operator()() const
        {
            std::system(to_str().data());
        }
        std::string to_str() const
        {
            std::string str = cmmd;
            for (std::string i : args)
                str += " " + i;
            return str;
        }
    };
    // }
}
#endif
