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

int get_ip_addr(const char *iface, char *ipstr)
{
	int fd, rv;
	struct ifreq ifr;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Failed to open socket: %s\n", strerror(errno));
		return -1;
	}

	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

	if ((rv = ioctl(fd, SIOCGIFADDR, &ifr)) == -1) {
		printf("IOCTL error: %s\n", strerror(errno));
		return -1;
	}

	close(fd);

	sprintf(ipstr, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	return 0;
}
