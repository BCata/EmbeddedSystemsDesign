#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <rtdm/gpio.h>

int main(int argc, char* argv[]) {
    int fd1, fd2, value;
    int ret1, ret2;

    fd1 = open("/dev/rtdm/pinctrl-bcm2835/gpio23", O_RDONLY|O_NONBLOCK);
    fd2 = open("/dev/rtdm/pinctrl-bcm2835/gpio22",O_WRONLY);

    //set device to input mode with special device request GPIO_RTIOC_DIR_IN
    ret1 = ioctl(fd1, GPIO_RTIOC_DIR_IN);
    //set device to output mode with special device request GPIO_RTIOC_DIR_OUT
    ret2 = ioctl(fd2, GPIO_RTIOC_DIR_OUT, &value);
    
    int i;

    for(i=0; i<100000; i++)
    {
        ret1 = read(fd1, &value, sizeof(value));
        // printf("Value from GPIO is %d\n", value);
        value = 1 - value;
        ret2 = write(fd2, &value, sizeof(value));
    }

    return 0;
}