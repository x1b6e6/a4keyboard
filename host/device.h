#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>

class KernelApi;

class Device
{
public:
    Device(KernelApi &api, int hid);

    int write(const uint8_t (&data)[64]);

    static std::optional<Device> find(KernelApi &api, std::string_view modalias,
            const std::function<bool(const std::vector<uint8_t> &descriptor)> &callback);

private:
    KernelApi &m_api;
    int m_hid{};
};
