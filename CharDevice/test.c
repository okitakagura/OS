#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_SIZE 1024
#define DEV_NAME "/dev/myDevice"

int main(void)
{
	int fd;
	char buf[MAX_SIZE];
	char get[MAX_SIZE];
	char devName[20];
	system("ls /dev/");
	printf("You wanna to use the \"myDevice\"");
	fgets(devName, 20, stdin);
	fd = open(DEV_NAME, O_RDWR | O_NDELAY);
	if (fd != -1)
	{
		read(fd, buf, sizeof(buf));
		printf("The device was inited with a string : %s\n", buf);
		 //测试写
		printf("Please input a string  :\n");
		fgets(get, MAX_SIZE, stdin);
		write(fd, get, sizeof(get));
		//测试读
		read(fd, buf, sizeof(buf)); 
		system("dmesg");
		printf("\nThe string in the device now is : %s\n", buf);
		close(fd);
		return 0;
	}
	else
	{
		printf("Device open failed\n");
		return -1;
	}
}

