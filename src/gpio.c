#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


static int gpio_write(const char *gpio_path, const char *value)
{
	int fd;

	if ((fd = open(gpio_path, O_WRONLY)) < 0) {
		printf("[%s:%d] Opening GPIO failed: %s\n"
			, __func__, __LINE__, strerror(errno));
		return -1;
	}

	if (write(fd, value, strlen(value)) < 0) {
		printf("[%s:%d] Write to gpio failed: %s\n"
		, __func__, __LINE__, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}

int gpio_reset_ssd1311(void)
{
	if (gpio_write("/sys/class/gpio/export", "18") != 0) {
		printf("[%s:%d] Failed to export GPIO\n", __func__, __LINE__);
		return -1;
	}

	if (gpio_write("/sys/class/gpio/gpio18/direction", "out") != 0) {
		printf("[%s:%d] Failed to set GPIO direction\n", __func__, __LINE__);
		return -1;
	}

	if (gpio_write("/sys/class/gpio/gpio18/value", "1") != 0) {
		printf("[%s:%d] Failed to set GPIO value\n", __func__, __LINE__);
		return -1;
	}

	usleep(50000);

	if (gpio_write("/sys/class/gpio/gpio18/value", "0") != 0) {
		printf("[%s:%d] Failed to set GPIO value\n", __func__, __LINE__);
		return -1;
	}

	usleep(50000);

	if (gpio_write("/sys/class/gpio/gpio18/value", "1") != 0) {
		printf("[%s:%d] Failed to set GPIO value\n", __func__, __LINE__);
		return -1;
	}

	return 0;
}

int gpio_reset_off(void)
{
	if (gpio_write("/sys/class/gpio/unexport", "18") != 0) {
		printf("[%s:%d] Failed to unexport GPIO\n", __func__, __LINE__);
		return -1;
	}
	return 0;
}
