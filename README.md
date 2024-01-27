# a4keyboard

Tool for controlling color of A4Tech Bloody keyboard

Rewritten [apo5tol project](https://github.com/apo5tol/bloody_keyboard_rgb_control) to use [HID-BPF](https://www.kernel.org/doc/html/next/hid/hid-bpf.html) for better user experience

<details>

<summary>Why not original project</summary>

---

Original apo5tol's `bloody_keyboard_rgb_control` project use [libusb](https://libusb.info/) library for communication with device.
This library must detach HID driver from device and only then device will be available to communication.
While HID driver is detached from device any keys pressed at this time will be missed.
Duration of original script more than 1 second - that is a lot.

Current project use HID-BPF what provides communication with device without detaching HID driver from device.
Any pressed keys while program is running will be correctly processed by HID driver.
Duration of my program is ~30ms (`color` command) whats take possibilities to make animations by user scripts (per key colors is not supported at now).

---

</details>

## Install dependencies

- Archlinux

  ```shell
  pacman -Sy --needed cmake clang ninja bpf libbpf lib32-gcc-libs
  ```

## Building

Only `ninja` generator is supported at now

Only `clang` compiler is supported at now

```shell
cmake -Bbuild -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++  \
    -DCMAKE_BPFTOOL=bpftool \
    -DCMAKE_INSTALL_PREFIX=/usr

# Install to the system. Files:
#   /usr/bin/a4keyboard
#   /usr/lib/udev/rules.d/80-a4keyboard.rules
sudo cmake --build build --target install/strip

# Set white color at boot (can be changed)
COLOR=FFFFFF
sed -e "s@init@color $COLOR@" /usr/lib/udev/rules.d/80-a4keyboard.rules | \
    sudo tee /etc/udev/rules.d/99-a4keyboard.rules

# Reload udev rules for automatic prepare keyboard after re-plug or at system boot
sudo udevamd control --reload

# prepare keyboard
a4keyboard init # or just reconnect keyboard
```

## Using

```shell
a4keyboard color f00 # set red color (short form)
a4keyboard color 00FF00 # set green color (long form)
```

## Tested devices

|   Type   |      Name     |  VID   |  PID   |
|----------|---------------|--------|--------|
| keyboard | A4Tech Bloody | `09DA` | `FA10` |
