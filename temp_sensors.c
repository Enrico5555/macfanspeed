#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main()
{
	int id = geteuid();
	if (id != 0)
	{
		printf("You must run this program as root\n");
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
	for (i = 1; i <= 23; i++)
	{
		sprintf(num, "%d", i);
		strcat(outname, num);
		strcat(labelname, num);
		strcat(labelname, "_label");
		strcat(outname, "_input");
		strcat(fpath, labelname);
		int lfile = open(fpath, O_RDONLY);
		memset(val, 0, 64);
		read(lfile, val, 64);
		close(lfile);
		printf("Sensor label: %s", val);
		fpath[len] = 0;
		strcat(fpath, outname);
		int ofile = open(fpath, O_RDONLY);
		memset(val, 0, 64);
		read(ofile, val, 64);
		close(ofile);
		//printf("Sensor value: %s\n", val);
		fpath[len] = 0;
		outname[4] = 0;
		labelname[4] = 0;
	}
}
