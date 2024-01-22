#pragma once

class Device;
struct Color;

namespace cmd {

int init(Device &dev);
int setColor(Device &dev, const Color &color);

} // namespace cmd
