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

int Device::write(const uint8_t (&data)[64])
{
    return m_api.write(m_hid, data);
}

std::optional<Device> Device::find(KernelApi &api, std::string_view modalias,
        const std::function<bool(const std::vector<uint8_t> &descriptor)> &callback)
{
    for (const auto &entry : std::filesystem::directory_iterator("/sys/bus/hid/devices")) {
        if (!entry.is_directory())
            continue;

        auto path = entry.path();
        {
            std::string s;
            std::ifstream(path / "modalias") >> s;

            if (std::strcmp(s.c_str(), modalias.data()) != 0)
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
            if (!callback(descriptor))
                continue;
        }

        std::string filename = path.filename().string();
        int hid = std::strtol(filename.c_str() + 15, nullptr, 16);
        return Device(api, hid);
    }

    return std::nullopt;
}
