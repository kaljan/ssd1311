#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

const char i2c_fname[]="/dev/i2c-1";
int i2c_fd;

int i2c_send_data(uint8_t addr, const uint8_t *buf, int size)
{
	if ((i2c_fd = open(i2c_fname, O_RDWR)) < 0) {
		printf("Failed to open %s: %s\n", i2c_fname, strerror(errno));
		return -1;
	}

	if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
		close(i2c_fd);
		printf("I2C Connection failed: %s\n", strerror(errno));
		return -1;
	}

	if (write(i2c_fd, buf, size) < 0) {
		close(i2c_fd);
		printf("[%s, %d] I2C write failed: %s\n"
		, __func__, __LINE__, strerror(errno));;
		return -1;
	}

	close(i2c_fd);

	return 0;
}
