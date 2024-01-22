#include "device.h"

#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include "kernelapi.h"

Device::Device(KernelApi &api, int hid)
    : m_api(api)
    , m_hid(hid)
{
}

Device::Device(Device &&other)
    : m_api(other.m_api)
{
    std::swap(m_hid, other.m_hid);
}

int Device::write(const uint8_t (&data)[64])
{
    return m_api.write(m_hid, data);
}

std::optional<Device> Device::find(KernelApi &api, const std::set<std::string_view> &modaliases,
        const std::function<bool(
                const std::string &modalias, const std::vector<uint8_t> &descriptor)> &callback)
{
    for (const auto &entry : std::filesystem::directory_iterator("/sys/bus/hid/devices")) {
        if (!entry.is_directory())
            continue;

        std::string modalias;
        auto path = entry.path();
        {
            std::ifstream(path / "modalias") >> modalias;

            if (modaliases.count(modalias) == 0)
                continue;
        }
        {
            auto uevent = path / "uevent";
            int fd = ::open(uevent.c_str(), O_RDONLY | O_NONBLOCK);
            if (fd < 0)
                continue;

            ::close(fd);
        }
        {
            auto desc = path / "report_descriptor";
            int fd = ::open(desc.c_str(), O_RDONLY | O_NONBLOCK);
            if (fd < 0)
                continue;

            unsigned char buf[BUFSIZ];
            size_t size = ::read(fd, buf, BUFSIZ);

            ::close(fd);

            std::vector<uint8_t> descriptor(buf, buf + size);
            if (!callback(modalias, descriptor))
                continue;
        }

        std::string filename = path.filename().string();
        int hid = std::strtol(filename.c_str() + 15, nullptr, 16);
        return Device{api, hid};
    }

    return std::nullopt;
}
