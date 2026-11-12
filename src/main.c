#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/atomic.h>

static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec y0   = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

static atomic_t led_state = ATOMIC_INIT(0);

void thread_led_blinky(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	if (!gpio_is_ready_dt(&led3)) {
		return;
	}
	gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);

	while (1) {
		gpio_pin_set_dt(&led3, 1);
		atomic_set(&led_state, 1);
		k_msleep(2000);

		gpio_pin_set_dt(&led3, 0);
		atomic_set(&led_state, 0);
		k_msleep(2000);
	}
}
void thread_y0_follow(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	atomic_val_t last = -1;   

	if (!gpio_is_ready_dt(&y0)) {
		return;
	}
	gpio_pin_configure_dt(&y0, GPIO_OUTPUT_INACTIVE);

	while (1) {
		atomic_val_t now = atomic_get(&led_state);

		if (now != last) {         
			gpio_pin_set_dt(&y0, (int)now);
			last = now;
		}
		k_msleep(10);
	}
}

K_THREAD_DEFINE(led_blinky_id, 1024, thread_led_blinky, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(y0_follow_id,  1024, thread_y0_follow,  NULL, NULL, NULL, 7, 0, 0);
