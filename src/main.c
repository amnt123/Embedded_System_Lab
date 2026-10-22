#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_EXISTS(LED_NODE)
#error "No led0 alias defined in devicetree"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

void thread_led_blinky(void *p1, void *p2, void *p3)

{

    ARG_UNUSED(p1);

    ARG_UNUSED(p2);

    ARG_UNUSED(p3);

 

    if (!gpio_is_ready_dt(&led)) {

        return;

    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {

        return;

    }

 

    while (1) {

        gpio_pin_set_dt(&led, 1);

        k_msleep(1000);

        gpio_pin_set_dt(&led, 0);

        k_msleep(2000);

    }

}

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led)) {
        printk("LED GPIO controller is not ready\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure LED: %d\n", ret);
        return 0;
    }

    printk("Zephyr STM32F4 Discovery: toggling LED on PE4\n");


    return 0;
}
