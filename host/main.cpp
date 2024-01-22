#include <unistd.h>

#include "args.h"
#include "cmd.h"
#include "device.h"
#include "kernelapi.h"

static constexpr std::string_view modalias = "hid:b0003g0001v000009DAp0000FA10";

static bool descriptor(const std::vector<uint8_t> &desc)
{
    return desc.size() == 133;
}

int main(int argc, const char **argv)
{
    Args args(argc, argv);

    switch (args.command()) {
    case Command::init: {
        setuid(0);
        KernelApi api{({
            auto api = KernelApi::create();
            if (!api.has_value())
                return -ENOENT;
            *api;
        })};
        Device dev{({
            auto dev = Device::find(api, modalias, descriptor);
            if (!dev.has_value())
                return -ENODEV;
            *dev;
        })};
        return cmd::init(dev);
    }
    case Command::setColor: {
        auto color = args.color();
        if (!color.has_value()) {
            args.print_help();
            return 1;
        }
        setuid(0);
        KernelApi api{({
            auto api = KernelApi::create();
            if (!api.has_value())
                return -ENOENT;
            *api;
        })};
        Device dev{({
            auto dev = Device::find(api, modalias, descriptor);
            if (!dev.has_value())
                return -ENODEV;
            *dev;
        })};
        return cmd::setColor(dev, color.value());
    }
    default:
        args.print_help();
        return 1;
    }
}
