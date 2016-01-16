#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FAN_MAX 6200
#define FAN_MIN 2000
#define WAIT_TIME 5

static int getsensorvalue(const char *fpath)
{
    char buf[8];
    memset(buf, 0, 8);
    int val;
    int file = open(fpath, O_RDONLY);
    read(file, buf, 8);
    val = atoi(buf);
    close(file);
    return val;
}

static void setfanspeed(int parameter)
{
    if (parameter < FAN_MIN)
    {
        parameter = FAN_MIN;
    }
    if (parameter > FAN_MAX)
    {
        parameter = FAN_MAX;
    }
    printf("Setting fanspeed to %d\n", parameter);
    int file;
    file = open("/sys/devices/platform/applesmc.768/fan1_manual", O_WRONLY | O_CREAT);
    char buf[8];
    buf[0] = '1';
    write(file, buf, (ssize_t)1);
    close(file);
    
    sprintf(buf, "%d", parameter);

    file = open("/sys/devices/platform/applesmc.768/fan1_output", O_WRONLY | O_CREAT);
    write(file, buf, (ssize_t)4);
    close(file);
}

int main(int argc, char **argv)
{
	int id = geteuid();
	if (id != 0)
	{
		printf("You must run this program as root\n");
		return -1;
	}
	if (argc != 4)
	{
        printf("error, usage: fanspeed [sensor_name] [minval] [maxval]\n");
        return -1;
	}
	char num[16];
	char labelname[64], outname[64];
	strcpy(outname, "temp");
	strcpy(labelname, "temp");
	char fpath[128];
	strcpy(fpath, "/sys/devices/platform/applesmc.768/");
	int len = strlen(fpath);
	char val[64];
	int i;
	// find the file with label specified
	for (i = 1; i <= 23; i++)
	{
		sprintf(num, "%d", i);
		strcat(labelname, num);
		strcat(labelname, "_label");
		strcat(fpath, labelname);
		int lfile = open(fpath, O_RDONLY);
		memset(val, 0, 64);
		read(lfile, val, 64);
		close(lfile);
		val[4] = 0;
		printf("req sensor: %s\n", argv[1]);
		printf("Sensor label: %s\n", val);
		if (strcmp(argv[1], val) == 0) break;
		fpath[len] = 0;
		labelname[4] = 0;
	}
	if (i == 24) return -1;
    fpath[len] = 0;
	strcat(outname, num);
    strcat(outname, "_input");
    strcat(fpath, outname);
    float x1, x2;
    x1 = atof(argv[2]);
    x2 = atof(argv[3]);
    printf("x1: %f, x2: %f\n", x1, x2);
    float m = ((float)(FAN_MAX - FAN_MIN)) / (x2 - x1);
    float b = FAN_MIN - m*x1;
	time_t t = time(NULL);
	while (1)
	{
        float value = getsensorvalue(fpath) / 1000;
        float fx = value*m + b;
        printf("value: %f\n", value);
        printf("fx: %f\n", fx);
        setfanspeed(fx);
        while (t >= time(NULL) - WAIT_TIME) {}
        t = time(NULL);
	}
}
