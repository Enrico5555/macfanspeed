#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int id = geteuid();
    if (id != 0)
    {
        printf("You must run this program as root\n");
        return -1;
    }
    if (argc != 2)
    {
        printf("You must specify the rpm for the fan, value must be between 2000 and 6200\n");
        return -1;
    }
    int parameter = atoi(argv[1]);
    
    if (parameter >= 2000 && parameter <= 6200)
    {
        int file;
        file = open("/sys/devices/platform/applesmc.768/fan1_manual", O_WRONLY | O_CREAT);
        write(file, "1", (ssize_t)1);
        close(file);
        
        file = open("/sys/devices/platform/applesmc.768/fan1_output", O_WRONLY | O_CREAT);
        write(file, argv[1], (ssize_t)4);
        close(file);
        printf("Fan RPM changed to %s\n", argv[1]);
    }
    else
    {
        printf("Value must be between 2000 and 6200\n");
    }
    return 0;
}
