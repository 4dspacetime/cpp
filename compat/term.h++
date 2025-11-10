#ifndef COMPAT_TERM_HPP
#define COMPAT_TERM_HPP
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
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
#else
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
    template <std::size_t width, std::size_t height>
    struct frm
    {
        std::array<std::array<char, width>, height> data;
        frm()
        {
            clear();
        }
        char at
        (
            const std::size_t x,
            const std::size_t y
        )
        {
            return x < width && y < height ? data[y][x] : '\0';
        }
        void clear()
        {
            draw_all(' ');
        }
        void draw_all(const char chr)
        {
            for (std::array<char, width> &i : data)
                i.fill(chr);
        }
        void draw_brd(const char chr)
        {
            draw_col(0, chr);
            draw_col(width  - 1, chr);
            draw_row(0, chr);
            draw_row(height - 1, chr);
        }
        void draw_col
        (
            const std::size_t pos,
            const char chr
        )
        {
            if (pos < width)
                for (std::array<char, width> &i : data)
                    i[pos] = chr;
        }
        template <std::size_t w, std::size_t h>
        void draw_frm(const frm<w, h> oth)
        {
            draw_frm(0, 0, oth);
        }
        template <std::size_t w, std::size_t h>
        void draw_frm
        (
            const std::size_t x,
            const std::size_t y,
            const frm<w, h> oth
        )
        {
            const std::size_t
                max_width  =
                    x < width  ? std::min(w, width  - x) : 0,
                max_height =
                    y < height ? std::min(h, height - y) : 0;
            for (std::size_t i = 0; i < max_width; i++)
                for (std::size_t j = 0; j < max_height; j++)
                    draw_pos(i + x, j + y, oth.data[j][i]);
        }
        void draw_pos
        (
            const std::size_t x,
            const std::size_t y,
            const char chr
        )
        {
            if (x < width && y < height)
                data[y][x] = chr;
        }
        void draw_row
        (
            const std::size_t pos,
            const char chr
        )
        {
            if (pos < height)
                data[pos].fill(chr);
        }
        void draw_sls
        (
            const std::size_t pos,
            const char chr,
            const char mode = '/'
        )
        {
            if (pos > width + height - 2)
                return;
            const enum { fwd, bck, oth } flg =
                mode == '/' ? fwd : mode == '\\' ? bck : oth;
            if (flg == oth)
                return;
            for (std::size_t i = 0; i < width; i++)
                draw_pos
                (
                    i,
                    flg == fwd ? pos - i : i - pos + height - 1,
                    chr
                );
        }
        void show() const
        {
            term::clear();
            for (const std::array<char, width> &i : data)
                std::cout.write(i.data(), width) << std::endl;
        }
    };
    // }
}
#endif
