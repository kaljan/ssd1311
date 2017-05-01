#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>

#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


extern char ssd1311_str[33];


int get_cpu_temp(void)
{
	static const char cpu_temperature_path[] =
		"/sys/class/thermal/thermal_zone0/temp\0";

	int fd;
	char cpu_tmp_str[8];
	int cpu_temp;
	memset(cpu_tmp_str, 0, 8);

	if ((fd = open(cpu_temperature_path, O_RDONLY)) < 0) {
		printf("[%s:%d] Can\'t open %s: %s\n"
			, __func__, __LINE__, cpu_temperature_path, strerror(errno));
		return -1;
	}

	if (read(fd, cpu_tmp_str, 8) < 0) {
		printf("[%s:%d] Can\'t read %s: %s\n"
			, __func__, __LINE__, cpu_temperature_path, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd);
	cpu_temp = atoi(cpu_tmp_str);

	return cpu_temp;
}

int show_cpu_temp(void)
{
	int cpu_temperature;
	cpu_temperature = get_cpu_temp();

	if (cpu_temperature > 0) {
		cpu_temperature /= 100;
		sprintf(&ssd1311_str[16], "CPU: %2d.%1d \'C    "
			, cpu_temperature / 10, cpu_temperature % 10);
	}
	return 0;
}
