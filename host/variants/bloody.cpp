#include "device.h"

class Bloody : public Device
{
public:
    using Device::Device;

    ~Bloody() override = default;

    int init() override;
    int setColor(Color) override;

    static bool probe(const std::vector<u8> &descriptor);
};

REGISTER_DEVICE(Bloody, 0x09DA, 0xFA10, Bloody::probe);

#define DEV_COMMAND(...)                                                                           \
    ({                                                                                             \
        int ret;                                                                                   \
        unsigned char buf[64];                                                                     \
        unsigned char arg[] = {0x07, __VA_ARGS__};                                                 \
        __builtin_memset(buf, 0, sizeof(buf));                                                     \
        __builtin_memcpy(buf, arg, sizeof(arg));                                                   \
        ret = write(buf);                                                                          \
        if (ret < 0)                                                                               \
            return ret;                                                                            \
        ret;                                                                                       \
    })

#define DEV_SET_COLOR(color, dest)                                                                 \
    ({                                                                                             \
        int ret;                                                                                   \
        unsigned char buf[64];                                                                     \
        __builtin_memset(buf, color, sizeof(buf));                                                 \
        __builtin_memcpy(buf, dest, sizeof(dest));                                                 \
        ret = write(buf);                                                                          \
        if (ret < 0)                                                                               \
            return ret;                                                                            \
        ret;                                                                                       \
    })

int Bloody::init()
{
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x05);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x29);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x05);
    DEV_COMMAND(0x07);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x2a);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x1f);
    DEV_COMMAND(0x2a);
    DEV_COMMAND(0x1e, 0x01);
    DEV_COMMAND(0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);
    DEV_COMMAND(0x05);
    DEV_COMMAND(0x2f, 0x00, 0x2e);
    DEV_COMMAND(0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80);
    DEV_COMMAND(0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80);
    DEV_COMMAND(0x03, 0x06, 0x05);
    DEV_COMMAND(0x06);
    DEV_COMMAND(0x03, 0x06, 0x01);
    DEV_COMMAND(0x03, 0x06, 0x01);
    DEV_COMMAND(0x03, 0x06, 0x05);
    DEV_COMMAND(0x03, 0x06, 0x01);
    DEV_COMMAND(0x03, 0x06, 0x01);
    DEV_COMMAND(0x05);

    return 0;
}

static unsigned char R1[] = {0x07, 0x03, 0x06, 0x07, 0x00, 0x00};
static unsigned char R2[] = {0x07, 0x03, 0x06, 0x08, 0x00, 0x00};
static unsigned char G1[] = {0x07, 0x03, 0x06, 0x09, 0x00, 0x00};
static unsigned char G2[] = {0x07, 0x03, 0x06, 0x0a, 0x00, 0x00};
static unsigned char B1[] = {0x07, 0x03, 0x06, 0x0b, 0x00, 0x00};
static unsigned char B2[] = {0x07, 0x03, 0x06, 0x0c, 0x00, 0x00};

int Bloody::setColor(Color color)
{
    DEV_SET_COLOR(color.r, R1);
    DEV_SET_COLOR(color.r, R2);
    DEV_SET_COLOR(color.g, G1);
    DEV_SET_COLOR(color.g, G2);
    DEV_SET_COLOR(color.b, B1);
    DEV_SET_COLOR(color.b, B2);

    DEV_COMMAND(0x05);

    return 0;
}

bool Bloody::probe(const std::vector<u8> &descriptor)
{
    // TODO: parse and check data inside

    return descriptor.size() == 133;
}
