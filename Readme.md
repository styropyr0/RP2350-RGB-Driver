# Xiao RP2350's WS2812 RGB Controller

This project demonstrates how to control the onboard WS2812 (NeoPixel) LED of the **Seeed Studio XIAO RP2350** using C/C++ with the Pico SDK.

 The provided code works with the [COLORMOD desktop application](https://github.com/styropyr0/oled.h)
 and also with any tool capable of sending raw data over a serial port (such as Arduino Serial Monitor, PuTTY, or other terminal programs).

---

## Features

* Control the **built-in WS2812 LED** on the XIAO RP2350.
* Supports **RGB color mixing** with 24-bit color values (8 bits per channel).
* Compatible with the **COLORMOD app** for live color changes via sliders.
* Can also be controlled from any serial terminal by sending raw color data.
* Simple, lightweight code with minimal dependencies.

---

## Hardware

* **Board:** Seeed Studio XIAO RP2350
* **LED:** Built-in single WS2812 NeoPixel (RGB only, no dedicated white channel)

If you want to control an **external WS2812 strip or LED**, simply replace the LED pin number in the code with the pin connected to your strip.

---

## Color Format

The WS2812 uses a **GRB** format, not standard RGB.
The color is expressed as a **24-bit unsigned integer**:

```
0x00GGRRBB
```

In code, it is constructed as:

```c
uint32_t color = (g << 16) | (r << 8) | (b << 0);
```

* **g** = green (0–255)
* **r** = red (0–255)
* **b** = blue (0–255)

> Note: The WS2812 on XIAO RP2350 does **not** support a dedicated white channel (W). If you set W, it will be ignored. To simulate white, set `r = g = b`.

---

## Usage with COLORMOD App

1. Upload the provided sketch to your XIAO RP2350.
2. Open the **COLORMOD application** on your PC.
3. Select the COM port corresponding to your board.
4. Leave the serial settings at default (9600 baud, 8N1).
5. Use the sliders to adjust **Red, Green, Blue** values.
6. The LED will update in real time.

---

## Usage with Serial Terminals

The LED can also be controlled via any serial program that sends raw data. Examples include:

* Arduino IDE Serial Monitor
* PuTTY
* screen / minicom (Linux)

### Command Format

Send **3 bytes** representing RGB values in order:

```
[RED] [GREEN] [BLUE]
```

Each value is a single byte (0–255).

For example:

* Sending `255 0 0` → LED turns Red
* Sending `0 255 0` → LED turns Green
* Sending `0 0 255` → LED turns Blue
* Sending `255 255 255` → LED turns White (simulated)

---

## Replacing the LED Pin

By default, the code uses the onboard NeoPixel pin.
If you want to control an external WS2812, modify this line in the code:

```c
#define LED_PIN <your_gpio_pin>
```

Connect the external LED strip’s **DIN** pin to the chosen GPIO.

---

## Example Code Snippet

```c
#include "ws2812.pio.h"

#define LED_PIN 23  // Onboard WS2812 pin

void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = (g << 16) | (r << 8) | (b << 0);
    put_pixel(color);
}
```

---

## License

This project is open-source and can be freely used or modified.