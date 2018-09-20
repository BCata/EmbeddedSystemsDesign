#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <rtdm/gpio.h>

RT_TASK button_task;

void action_led(void *arg) {
    int fd;
    int ret;
    int counter = 0;
    int value = *(int *)arg;

    fd = open("/dev/rtdm/pinctrl-bcm2835/gpio22",O_WRONLY);
    ret = ioctl(fd, GPIO_RTIOC_DIR_OUT, &value);  
    if(ret < 0) {
        rt_printf("Error with ioctl\n\n");
        exit(1);
    }
    
    value = 1-value;
    ret = write(fd, &value, sizeof(value)); 
    if(ret < 0) {
        rt_printf("Error %d when writing GPIO22\n", ret);
        exit(1);
    }
}

void button(void *arg) {
    int fd,value;
    int counter = 0;
    int ret;
    int xeno_trigger=GPIO_TRIGGER_EDGE_FALLING;
    
    fd = open("/dev/rtdm/pinctrl-bcm2835/gpio23",O_RDONLY);
    ret = ioctl(fd, GPIO_RTIOC_IRQEN, &xeno_trigger);
    if(ret < 0) {
        rt_printf("Error with ioctl\n\n");
        exit(1);
    }
    
    while(1) {
        ret = read(fd, &value, sizeof(value));
        if(ret < 0) {
            rt_printf("Error %d when reading GPIO23\n", ret);
            exit(3);
        }

        counter ++;
        value = counter % 2;
        printf("Counter is: %d\n", counter);
        action_led(&value);
    }
}

int main(int argc, char* argv[]) {
    char str[10];

    sprintf(str, "button_task");

    rt_task_create(&button_task, str, 0, 99, 0);
    rt_task_start(&button_task, &button, 0);

    printf("\nType CTRL-C to end this program\n\n" );
    pause();
    return 0;
}