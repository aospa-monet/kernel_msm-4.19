/*
 * platform indepent driver interface
 *
 * Coypritht (c) 2017 Goodix
 */
#define DEBUG
#define pr_fmt(fmt)     "gf_platform: " fmt

#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/timer.h>
#include <linux/err.h>

#include "gf_spi.h"

#if defined(USE_SPI_BUS)
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#elif defined(USE_PLATFORM_BUS)
#include <linux/platform_device.h>
#endif

int gf_parse_dts_ta(struct gf_dev *gf_dev)
{
	/*get reset resource*/
	gf_dev->reset_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node,
						"goodix,gpio-reset", 0);

	if (!gpio_is_valid(gf_dev->reset_gpio)) {
		pr_info("RESET GPIO is invalid.\n");
		return -EPERM;
	}

	/*get irq resourece*/
	gf_dev->irq_gpio = of_get_named_gpio(gf_dev->spi->dev.of_node,
						"goodix,gpio-irq", 0);
	pr_info("gf::irq_gpio:%d\n", gf_dev->irq_gpio);

	if (!gpio_is_valid(gf_dev->irq_gpio)) {
		pr_info("IRQ GPIO is invalid.\n");
		return -EPERM;
	}

	return 0;
}

void gf_cleanup_ta(struct gf_dev *gf_dev)
{
	pr_info("[info] %s\n", __func__);

	if (gpio_is_valid(gf_dev->irq_gpio)) {
		gpio_free(gf_dev->irq_gpio);
		pr_info("remove irq_gpio success\n");
	}

	if (gpio_is_valid(gf_dev->reset_gpio)) {
		gpio_free(gf_dev->reset_gpio);
		pr_info("remove reset_gpio success\n");
	}

}

int gf_power_on_ta(struct gf_dev *gf_dev)
{
	int rc = 0;
	msleep(10);
	return rc;
}

int gf_power_off_ta(struct gf_dev *gf_dev)
{
	int rc = 0;
	return rc;
}

int gf_hw_reset_ta(struct gf_dev *gf_dev, unsigned int delay_ms)
{
	if (gf_dev == NULL) {
		pr_info("Input buff is NULL.\n");
		return -EPERM;
	}

	gpio_direction_output(gf_dev->reset_gpio, 0);
	mdelay(3);
	gpio_set_value(gf_dev->reset_gpio, 1);
	mdelay(delay_ms);
	pr_info("%s\n", __func__);
	return 0;
}

int gf_irq_num_ta(struct gf_dev *gf_dev)
{
	if (gf_dev == NULL) {
		pr_info("Input buff is NULL.\n");
		return -EPERM;
	} else {
		return gpio_to_irq(gf_dev->irq_gpio);
	}
}

