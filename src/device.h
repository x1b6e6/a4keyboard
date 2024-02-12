#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>

#include "color.h"

class KernelApi;

using VidPid = std::pair<u16, u16>;

#define REGISTER_DEVICE(DeviceClass, vid, pid, probe)                                              \
    namespace {                                                                                    \
    static const int _##DeviceClass##Type =                                                        \
            ::Device::registerDevice<DeviceClass>({vid, pid}, probe);                              \
    }

class Device
{
    struct RegisteredDevice {
        [[nodiscard]] bool (*probe)(const std::vector<u8> &descriptor);
        [[nodiscard]] Device *(*make)(KernelApi &, u32 hid, const std::vector<u8> &descriptor);
    };

protected:
    Device(KernelApi &api, u32 hid, const std::vector<u8> &descriptor);

    int write(const u8 (&data)[64]);

public:
    Device(const Device &) = delete;
    Device(Device &&);

    virtual ~Device() = default;

    [[nodiscard]] virtual int init() = 0;
    [[nodiscard]] virtual int setColor(Color) = 0;

    static std::optional<std::unique_ptr<Device>> find(KernelApi &api);

    template<typename T>
    static int registerDevice(VidPid, bool (*probe)(const std::vector<u8> &descriptor));

private:
    static std::optional<RegisteredDevice> getRegistered(VidPid);

    KernelApi &m_api;
    u32 m_hid{};

    static std::map<VidPid, RegisteredDevice> m_registered;
};

template<typename T>
int Device::registerDevice(VidPid vidPid, bool (*probe)(const std::vector<u8> &descriptor))
{
    m_registered.insert({vidPid,
            RegisteredDevice{
                    .probe = probe,
                    .make = [](KernelApi &k, u32 hid, const std::vector<u8> &descriptor)
                            -> Device * { return new T(k, hid, descriptor); },
            }});
    return 0;
}
