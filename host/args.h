#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum struct Command {
    unknown = 0,
    init,
    setColor,
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Args
{
public:
    Args(int argc, const char **argv);

    Command command() const;

    std::optional<Color> color() const;

    void print_help() const;

private:
    std::string m_progname;
    Command m_command{};
    std::optional<Color> m_color;
};
