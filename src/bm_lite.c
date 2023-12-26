#include "bm_lite.h"
#include "main.c"


void platform_bmlite_reset(void)     //reset the chip for 100ms
{
    gpio_pin_set(dev, RST_PIN, 0);
    k_sleep(K_MSEC(100));
    gpio_pin_set(dev, RST_PIN, 1);
    k_sleep(K_MSEC(100));


        gpio_pin_set(dev, RST_PIN, 0);
    k_sleep(K_MSEC(100));
    gpio_pin_set(dev, RST_PIN, 1);
    k_sleep(K_MSEC(100));



        gpio_pin_set(dev, RST_PIN, 0);
    k_sleep(K_MSEC(100));
    gpio_pin_set(dev, RST_PIN, 1);
    k_sleep(K_MSEC(100));


        gpio_pin_set(dev, RST_PIN, 0);
    k_sleep(K_MSEC(100));
    gpio_pin_set(dev, RST_PIN, 1);
    k_sleep(K_MSEC(100));
}



