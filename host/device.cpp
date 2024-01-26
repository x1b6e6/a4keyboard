#include "device.h"

#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include "kernelapi.h"

std::map<std::pair<u16, u16>, Device::RegisteredDevice> Device::m_registered;

Device::Device(KernelApi &api, u32 hid, const std::vector<u8> &descriptor)
    : m_api(api)
    , m_hid(hid)
{
}

Device::Device(Device &&other)
    : m_api(other.m_api)
{
    std::swap(m_hid, other.m_hid);
}

int Device::write(const u8 (&data)[64])
{
    return m_api.write(m_hid, data);
}

std::optional<Device::RegisteredDevice> Device::getRegistered(VidPid vidPid)
{
    auto it = m_registered.find(vidPid);
    if (it == m_registered.end())
        return std::nullopt;
    return it->second;
}

std::optional<std::unique_ptr<Device>> Device::find(KernelApi &api)
{
    for (const auto &entry : std::filesystem::directory_iterator("/sys/bus/hid/devices")) {
        if (!entry.is_directory())
            continue;

        auto path = entry.path();

        auto dirname = path.filename();
        if (std::strlen(dirname.c_str()) != 19)
            continue;

        u16 vid = std::strtol(dirname.c_str() + 5, nullptr, 16);
        u16 pid = std::strtol(dirname.c_str() + 10, nullptr, 16);

        auto registered = getRegistered({vid, pid});
        if (!registered.has_value())
            continue;

        auto desc = path / "report_descriptor";
        int fd = ::open(desc.c_str(), O_RDONLY | O_NONBLOCK);
        if (fd < 0)
            continue;

        unsigned char buf[BUFSIZ];
        size_t size = ::read(fd, buf, BUFSIZ);

        ::close(fd);

        std::vector<uint8_t> descriptor(buf, buf + size);
        if (!registered->probe(descriptor))
            continue;

        std::string filename = path.filename().string();
        int hid = std::strtol(filename.c_str() + 15, nullptr, 16);

        return std::unique_ptr<Device>{registered->make(api, hid, descriptor)};
    }

    return std::nullopt;
}
