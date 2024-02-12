#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "args.h"
#include "device.h"
#include "kernelapi.h"

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

    if (ret < 0)
        std::cerr << "Error: " << strerror(-ret) << std::endl;

    return ret;
}

template<typename F>
static inline int run(F f)
{
    if (getuid() != 0 && setuid(0) != 0)
        std::cerr << "Warn: Cannot get root permissions" << std::endl;

    auto api = KernelApi::create();
    if (!api.has_value())
        throw std::runtime_error("Cannot connect to kernel");

    auto dev = Device::find(*api);
    if (!dev.has_value())
        throw std::runtime_error("Cannot find device");

    int ret = f(*dev);

    return ret;
}

static int runCommand(const Args &args)
{
    switch (args.command()) {
    case Command::init:
        return run([](const std::unique_ptr<Device> &dev) { return dev->init(); });
    case Command::setColor: {
        auto color = args.color();
        if (!color.has_value()) {
            args.print_help();
            return 1;
        }

        using namespace std::placeholders;

        return run([&](const std::unique_ptr<Device> &dev) { return dev->setColor(*color); });
    }
    default:
        args.print_help();
        return 1;
    }
}
