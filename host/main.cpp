#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "args.h"
#include "cmd.h"
#include "device.h"
#include "kernelapi.h"

static constexpr std::string_view bloodyE975 = "hid:b0003g0001v000009DAp0000FA10";

static bool descriptor(const std::string &modalias, const std::vector<uint8_t> &desc)
{
    if (modalias == bloodyE975)
        return desc.size() == 133;

    return false;
}

static int runCommand(const Args &args);

int main(int argc, const char **argv)
{
    Args args(argc, argv);

    int ret{};

    try {
        ret = runCommand(args);

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (ret != 0)
        std::cerr << "Error: " << strerror(-ret) << std::endl;

    return ret;
}

template<typename F>
static int run(F f)
{
    if (getuid() != 0 && setuid(0) != 0)
        throw std::runtime_error("permissions");

    auto api = KernelApi::create();
    if (!api.has_value())
        throw std::runtime_error("Cannot connect to kernel");

    auto dev = Device::find(*api, {bloodyE975}, descriptor);
    if (!dev.has_value())
        throw std::runtime_error("Cannot find device");

    int ret = f(*dev);

    return ret;
}

static int runCommand(const Args &args)
{
    switch (args.command()) {
    case Command::init:
        return run(cmd::init);
    case Command::setColor: {
        auto color = args.color();
        if (!color.has_value()) {
            args.print_help();
            return 1;
        }

        using namespace std::placeholders;

        return run(std::bind(cmd::setColor, _1, color.value()));
    }
    default:
        args.print_help();
        return 1;
    }
}
