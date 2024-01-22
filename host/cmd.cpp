#include "cmd.h"

#include "args.h"
#include "device.h"

#define DEV_COMMAND(dev, ...)                                                                      \
    ({                                                                                             \
        int ret;                                                                                   \
        unsigned char buf[64];                                                                     \
        unsigned char arg[] = {0x07, __VA_ARGS__};                                                 \
        __builtin_memset(buf, 0, sizeof(buf));                                                     \
        __builtin_memcpy(buf, arg, sizeof(arg));                                                   \
        ret = dev.write(buf);                                                                      \
        if (ret < 0)                                                                               \
            return ret;                                                                            \
        ret;                                                                                       \
    })

#define DEV_SET_COLOR(dev, color, dest)                                                            \
    ({                                                                                             \
        int ret;                                                                                   \
        unsigned char buf[64];                                                                     \
        __builtin_memset(buf, color, sizeof(buf));                                                 \
        __builtin_memcpy(buf, dest, sizeof(dest));                                                 \
        ret = dev.write(buf);                                                                      \
        if (ret < 0)                                                                               \
            return ret;                                                                            \
        ret;                                                                                       \
    })

namespace cmd {

int init(Device &dev)
{
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x05);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x29);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x05);
    DEV_COMMAND(dev, 0x07);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x2a);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x1f);
    DEV_COMMAND(dev, 0x2a);
    DEV_COMMAND(dev, 0x1e, 0x01);
    DEV_COMMAND(dev, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);
    DEV_COMMAND(dev, 0x05);
    DEV_COMMAND(dev, 0x2f, 0x00, 0x2e);
    DEV_COMMAND(dev, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80);
    DEV_COMMAND(dev, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80);
    DEV_COMMAND(dev, 0x03, 0x06, 0x05);
    DEV_COMMAND(dev, 0x06);
    DEV_COMMAND(dev, 0x03, 0x06, 0x01);
    DEV_COMMAND(dev, 0x03, 0x06, 0x01);
    DEV_COMMAND(dev, 0x03, 0x06, 0x05);
    DEV_COMMAND(dev, 0x03, 0x06, 0x01);
    DEV_COMMAND(dev, 0x03, 0x06, 0x01);
    DEV_COMMAND(dev, 0x05);

    return 0;
}

static unsigned char R1[] = {0x07, 0x03, 0x06, 0x07, 0x00, 0x00};
static unsigned char R2[] = {0x07, 0x03, 0x06, 0x08, 0x00, 0x00};
static unsigned char G1[] = {0x07, 0x03, 0x06, 0x09, 0x00, 0x00};
static unsigned char G2[] = {0x07, 0x03, 0x06, 0x0a, 0x00, 0x00};
static unsigned char B1[] = {0x07, 0x03, 0x06, 0x0b, 0x00, 0x00};
static unsigned char B2[] = {0x07, 0x03, 0x06, 0x0c, 0x00, 0x00};

int setColor(Device &dev, const Color &color)
{
    DEV_SET_COLOR(dev, color.r, R1);
    DEV_SET_COLOR(dev, color.r, R2);
    DEV_SET_COLOR(dev, color.g, G1);
    DEV_SET_COLOR(dev, color.g, G2);
    DEV_SET_COLOR(dev, color.b, B1);
    DEV_SET_COLOR(dev, color.b, B2);

    DEV_COMMAND(dev, 0x05);

    return 0;
}

} // namespace cmd
