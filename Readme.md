# Xiao RP2350's WS2812 RGB Controller

This project demonstrates how to control the onboard WS2812 (NeoPixel) LED of the **Seeed Studio XIAO RP2350** using C/C++ with the Pico SDK.

The provided code works with the [COLORMOD desktop application](https://github.com/styropyr0/ColorMod) and with any tool capable of sending integer values over a serial port (such as Arduino Serial Monitor, PuTTY, or other terminal programs).

---

## Features

* Control the **built-in WS2812 LED** on the XIAO RP2350.
* Supports **R, G, B, W channels** through integer-based commands.
* Compatible with the **COLORMOD app** for interactive control.
* Can also be controlled from any serial terminal.
* Includes a **handshake step** to ensure communication with the host.

---

## Hardware

* **Board:** Seeed Studio XIAO RP2350
* **LED:** Built-in WS2812 NeoPixel (RGB — no dedicated hardware white channel)

If you want to control an **external WS2812 LED/strip**, replace the LED pin in the code:

```c
#define WS2812_PIN <your_gpio_pin>
```

---

## Communication Protocol

After reset, the firmware waits for a **handshake value of `9090`** from the PC.
Once received, it accepts integer commands:

### Accepted Values

| Range      | Effect                                                |
| ---------- | ----------------------------------------------------- |
| `1–255`    | Sets **Red** intensity (R = value)                    |
| `256–511`  | Sets **Green** intensity (G = value - 256)            |
| `512–767`  | Sets **Blue** intensity (B = value - 512)             |
| `768–1023` | Sets **White** intensity (W = value - 768, simulated) |
| `1024`     | Turns **off** the LED                                 |
| Other      | Ignored (`NO EFFECT` message is printed)              |

### Example (via Serial Monitor)

```
9090       // Handshake
100        // Red = 100
300        // Green = 44
600        // Blue = 88
800        // White = 32 (simulated)
1024       // LED off
```

---

## Usage with COLORMOD App

1. Upload the provided sketch to your XIAO RP2350.
2. Open the **COLORMOD desktop app**.
3. Select the correct COM port.
4. The app will automatically send the handshake (`9090`).
5. Use sliders to adjust **Red, Green, Blue, White**.
6. LED updates in real time.

---

## Usage with Other Serial Tools

Any program capable of sending plain integer values over serial can control the LED:

* Arduino IDE Serial Monitor
* PuTTY
* screen / minicom (Linux)
* Custom .NET / Python scripts

Just send integers (follow the table above) ending with `\r\n` (newline).

---

## Notes

* The WS2812 LED on XIAO RP2350 does **not** have a dedicated white channel — the `W` value is simulated by mixing R, G, B.
* If you connect an **external RGBW WS2812**, the code already supports the `W` channel.

---

## License

This project is open-source and may be freely used or modified.
