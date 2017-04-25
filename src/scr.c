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

#include "ssd1311.h"


extern int gpio_reset_ssd1311(void);
extern int gpio_reset_off(void);

extern int end_app;


time_t t;
struct tm *tmptr;
char ssd1311_str[33];
int cpu_temperature;


int scr_init(void)
{
	if (gpio_reset_ssd1311() != 0) {
		printf("[%s:%d] Reset SSD1311 failed\n", __func__, __LINE__);
		return -1;
	}

	if (ssd1311_init() != 0) {
		printf("[%s:%d] Init SSD1311 failed\n", __func__, __LINE__);
		return -1;
	}

	if (ssd1311_on() != 0) {
		printf("[%s:%d] Running SSD1311 failed\n", __func__, __LINE__);
		return -1;
	}

	if (ssd1311_set_string("Hello World!\0", 0) != 0) {
		printf("[%s:%d] Write text on SSD1311 failed\n", __func__, __LINE__);
		return -1;
	}


}

int scr_deinit(void)
{
	if (gpio_reset_off() != 0) {
		return -1;
	}
}

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

int get_time(char *str)
{
	if (str == 0) {
		return -1;
	}
	t = time(NULL);
	tmptr = localtime(&t);
	strftime(str, 9, "%T ", tmptr);
	memset(&str[8], ' ', 24);
	return 0;
}

int show_date(void)
{
	char str[16];
	int l;

	strftime(str, 13, "%d-%b-%Y ", tmptr);
	l = strlen(str);
	memset(&str[l], ' ', 16 - l);
	strcpy(&ssd1311_str[16], str);
	return 0;
}

int show_week_day(void)
{
	char str[16];
	int l;

	strftime(str, 16, "%A\0", tmptr);
	l = strlen(str);
	memset(&str[l], ' ', 16 - l);
	strcpy(&ssd1311_str[16], str);
	return 0;
}

int show_cpu_temp(void)
{
	cpu_temperature = get_cpu_temp();

	if (cpu_temperature > 0) {
		cpu_temperature /= 100;
		sprintf(&ssd1311_str[16], "CPU: %2d.%1d \'C    "
			, cpu_temperature / 10, cpu_temperature % 10);
	}
	return 0;
}

int show_ip_addr(void)
{
	int fd;
	struct ifreq ifr;
	char str[16];
	int l;
	char iface[] = "wlan0";

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	//Type of address to retrieve - IPv4 IP address
	ifr.ifr_addr.sa_family = AF_INET;

	//Copy the interface name in the ifreq structure
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	//display result
	sprintf(str, "%s\0" ,
		inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	l = strlen(str);
	memset(&str[l], ' ', 16 - l);
	strcpy(&ssd1311_str[16], str);
	return 0;
}


struct _show_func {
	int (*show_func)(void);
	struct _show_func *next;
};

struct _show_func *sfptr;


void init_show(void)
{
	struct _show_func *top;
	top = malloc(sizeof(struct _show_func));
	if (top == 0) {
		printf("[%s:%d]Failed to allocate \'struct _show_func\'\n");
		return;
	}
	top->show_func = show_date;

	sfptr = malloc(sizeof(struct _show_func));
	if (sfptr == 0) {
		printf("[%s:%d]Failed to allocate \'struct _show_func\'\n");
		return;
	}

	top->next = sfptr;
	sfptr->show_func = show_week_day;

	sfptr->next = malloc(sizeof(struct _show_func));
	if (sfptr->next == 0) {
		printf("[%s:%d]Failed to allocate \'struct _show_func\'\n");
		return;
	}
	sfptr = sfptr->next;
	sfptr->show_func = show_cpu_temp;

#	if 0
	sfptr->next = malloc(sizeof(struct _show_func));
	if (sfptr->next == 0) {
		printf("[%s:%d]Failed to allocate \'struct _show_func\'\n");
		return;
	}
	sfptr = sfptr->next;
	sfptr->show_func = show_ip_addr;
#	endif
	sfptr->next = top;
	sfptr = top;
}

int animation(void)
{
	int i;

	i = sprintf(ssd1311_str, "PID: %d", getpid());
	memset(&ssd1311_str[i], ' ', 32 - i);
	ssd1311_set_string(ssd1311_str, 0);
	usleep(500000);

	i = 0;
	init_show();

	while (1) {
		usleep(500000);
		if (end_app == 1) {
			break;
		}

		get_time(ssd1311_str);

		(sfptr->show_func)();

		ssd1311_set_string(ssd1311_str, 0);

		i++;
		if (i > 60) {
			i = 0;
			sfptr = sfptr->next;
		}
	}

	sprintf(ssd1311_str, "EXIT");
	memset(&ssd1311_str[4], ' ', 28);
	ssd1311_set_string(ssd1311_str, 0);

	return 0;
}
