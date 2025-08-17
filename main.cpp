#include "pico/stdlib.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.asm.h"

#define PARSE_TIMEOUT_MS 1000

#define IS_RGBW true
#define NUM_PIXELS 1

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else

#define WS2812_PIN 22
#endif

bool connected = false;

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

int serial_parseInt()
{
    absolute_time_t start = get_absolute_time();
    int val = 0;
    bool started = false;

    while (true)
    {
        if (!connected)
            printf("FTERjhgdstkjlsa\r\n");
        
        fflush(stdout);

        int ch = getchar_timeout_us(1000);
        if (ch == PICO_ERROR_TIMEOUT)
        {
            if (started)
                if (absolute_time_diff_us(start, get_absolute_time()) > PARSE_TIMEOUT_MS * 1000)
                    return val;
            continue;
        }

        if (isdigit(ch))
        {
            val = val * 10 + (ch - '0');
            started = true;
            start = get_absolute_time();
        }
        else if (started)
            return val;
    }
}

long int val;
int r = 0, g = 0, b = 0, w = 0;

void setup()
{
    printf("Waiting for handshake signal with computer...\r\n");
    int handshake = 0;
    // put_pixel(g << 16 | r << 8 | b << 0 | 0);
    while (handshake != 9090)
        handshake = serial_parseInt();

    connected = true;
    printf("Handshake recieved\r\n");
    sleep_ms(1000);
    printf("COMMANDLINE CONTROL\n[0~768 for setting color] [1024 to turn off] [>1024 for dynamic lighting]\r\n");
}

void loop()
{
    val = serial_parseInt();
    printf("You entered: %ld\r\n", val);

    if (val < 256 && val > 0)
        r = val;
    else if (val > 255 && val < 512)
        g = val - 256;
    else if (val > 511 && val < 768)
        b = val - 512;
    else if (val > 767 && val < 1024)
        w = val - 768;
    else if (val == 1024)
        r = g = b = w = 0;
    else if (val != 0 && val != 1024)
        printf("NO EFFECT\r\n");

    put_pixel(w << 24 | g << 16 | r << 8 | b << 0 | 0);
}

int main()
{
    stdio_init_all();

    while (!stdio_usb_connected())
        sleep_ms(100);
        
    sleep_ms(2000);
    const int RGB_POWER = 23;
    gpio_init(RGB_POWER);
    gpio_set_dir(RGB_POWER, GPIO_OUT);
    gpio_put(RGB_POWER, 1);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    sleep_ms(2000);
    put_pixel(g << 16 | r << 8 | b << 0 | 0);
    setup();
    while (true)
        loop();
}