#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>

const int pinEn 	= 12;
const int pinPositive	= 4;
const int pinNegative	= 25;

// Functions
void exportDcMotor();
void unexportDcMotor();
void rotateDcMotor(int direction, int delay_sec);



void export(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
}

void unexport(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
}

void direction_in(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);	
	write(fd, "in", 3);
	close(fd);
}

void direction_out(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);	
	write(fd, "out", 4);
	close(fd);
}


void value_high(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	write(fd, "1", 1);
	close(fd);
}

void value_low(int gpio)
{
	int fd;
	char buf[BUFSIZ];
	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	write(fd, "0", 1);
	close(fd);
}

int gpio_read(int pin)
{
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}

void exportDcMotor() 
{
	export(pinEn);
	export(pinPositive);
	export(pinNegative);
	direction_out(pinEn);
	direction_out(pinPositive);
	direction_out(pinNegative);
}

void unexportDcMotor()
{
	unexport(pinEn);
	unexport(pinPositive);
	unexport(pinNegative);
}


void rotateDcMotor(int direction, int delay_sec)
{
	printf("Rotating DcMotor...\n");
	if(direction==0) {
		value_high(pinPositive);
		value_low(pinNegative);
	}
	else {
		value_high(pinNegative);
		value_low(pinPositive);
	}

	value_high(pinEn);
	sleep(delay_sec);

	value_low(pinEn);
	sleep(delay_sec);
	printf("Stopped DcMotor...\n");
}

