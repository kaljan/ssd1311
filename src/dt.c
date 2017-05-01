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

time_t t;
struct tm *tmptr;

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

