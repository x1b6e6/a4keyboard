#include "args.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

static inline std::optional<Color> parse_color(const char *str);

Args::Args(int argc, const char **argv)
{
    m_progname = argv[0];
    if (argc <= 1)
        return;

    switch (argv[1][0]) {
    case 'i':
        if (std::strcmp(argv[1], "init") == 0) {
            m_command = Command::init;
        }
        break;
    case 'c':
        if (std::strcmp(argv[1], "color") == 0) {
            m_command = Command::setColor;
            if (argc > 2) {
                m_color = parse_color(argv[2]);
            }
        }
        break;
    default:;
    }
}

Command Args::command() const
{
    return m_command;
}

std::optional<Color> Args::color() const
{
    return m_color;
}

void Args::print_help() const
{
    std::cerr << "Usage: " << m_progname << " COMMAND\n";
    std::cerr << "\n";
    std::cerr << "COMMAND:\n";
    std::cerr << "    init           initialize device\n";
    std::cerr << "    color COLOR    set color in HEX format\n";
    std::cerr << std::endl;
}

std::optional<Color> parse_color(const char *str)
{
    Color o;
    char buf[7];
    size_t len = std::strlen(str);

    switch (len) {
    case 3:
        buf[0] = buf[1] = str[0];
        buf[2] = buf[3] = str[1];
        buf[4] = buf[5] = str[2];
        buf[6] = '\0';
        break;
    case 6:
        __builtin_memcpy(buf, str, 7);
        break;
    default:
        std::cerr << "Error: incorrect color" << std::endl;
        return std::nullopt;
    }

    int value = std::strtol(buf, nullptr, 16);
    o.b = (value >> 0) & 0xff;
    o.g = (value >> 8) & 0xff;
    o.r = (value >> 16) & 0xff;

    return o;
}
