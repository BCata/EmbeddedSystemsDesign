#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <rtdm/gpio.h>

RT_TASK blink_task;

void blink(void *arg) {
    int fd1;
    int value = 1;
    int ret, err;

    fd1 = open("/dev/rtdm/pinctrl-bcm2835/gpio22",O_WRONLY);
    ret = ioctl(fd1, GPIO_RTIOC_DIR_OUT, &value);

    rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(5e8));

    while(1) {

        value = value % 2;
        rt_printf("Value: %d\n", value);

        ret = write(fd1, &value, sizeof(value));
        if(ret < 0)
        {
            rt_printf("Error %d when writing GPIO22\n", ret);
            exit(1);
        }

        value ++;

        err = rt_task_wait_period(NULL);
        if(err) {
            rt_printf("Error with rt_task_wait_period\n\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    char str[10];

    sprintf(str, "blink_task");

    rt_task_create(&blink_task, str, 0, 80, 0);
    rt_task_start(&blink_task, &blink, 0);

    printf("\nType CTRL-C to end this program\n\n" );
    pause();
}