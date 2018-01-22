#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>

#define SHT20_I2C_ADDR			0x40
#define SHT20_I2C_CMD_MEASURE_TEMP	0xF3
#define SHT20_I2C_CMD_MEASURE_HUMI	0xF5
#define SHT20_SOFT_RESET		0xFE
#define I2C_DEV_FILENAME		"/dev/i2c-1"
#define VALUE_MAX 30


float getTempData()
{
	float temp = 0;
	int fd;
	int i;
	uint32_t val=0;
	unsigned char data[3];
	unsigned char cmd_reset[1] = {SHT20_SOFT_RESET};
	unsigned char cmd_temp[1] = {SHT20_I2C_CMD_MEASURE_TEMP};

	fd = open( I2C_DEV_FILENAME, O_RDWR );
	if( fd < 0 )
	{
		printf("TEMP: Unable to initialize I2C\n");
		return -1;
	} 
	else 
	{
		ioctl( fd, I2C_SLAVE, SHT20_I2C_ADDR ); 
		if(write( fd, cmd_reset, 1 )<0){
			printf("TEMP : I2C Write error\n");
			close( fd );
			return -1;
		}
		usleep(50000);

		if(write( fd, cmd_temp, 1 )<0)
		{
			printf("TEMP : I2C Write error\n");
			close( fd );
			return 1;
		}
		usleep(260000);

		if(read( fd, data, 2 )<0)
		{
			printf("TEMP : I2C Read error\n");
			close( fd );
			return 1;
		}

		val = data[0] << 8 | data[1];
		temp = -46.85 + 175.72/65536*(int)val;
		printf("Temp : %.1f\n", (float)temp);
		close( fd );
	}

	return temp;
}

