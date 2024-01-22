#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <string_view>

class KernelApi;

class Device
{
public:
    Device(KernelApi &api, int hid);
    Device(const Device &) = delete;
    Device(Device &&);

    int write(const uint8_t (&data)[64]);

    static std::optional<Device> find(KernelApi &api, const std::set<std::string_view> &modaliases,
            const std::function<bool(const std::string &modalias,
                    const std::vector<uint8_t> &descriptor)> &callback);

private:
    KernelApi &m_api;
    int m_hid{};
};
