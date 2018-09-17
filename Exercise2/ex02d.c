#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>

RT_TASK task1, task2, task3;
RTIME now;

void demo(void *arg) {

    int err;

    RTIME period = * (RTIME *) arg;
    RTIME delay = 1000000000;

    err = rt_task_set_periodic(NULL, TM_NOW, rt_timer_ns2ticks(period));
    if (err) {
        printf("Error with rt_task_set_periodic\n\n");
    }
    
    rt_task_sleep(rt_timer_ns2ticks(delay));

    RT_TASK_INFO curtaskinfo;
    rt_task_inquire(NULL,&curtaskinfo);

    while (1) {
        now = rt_timer_read();
        rt_printf("Task name: %s\n", curtaskinfo.name);
        rt_printf("Exec time: %d\n", rt_timer_ticks2ns(now)/1000000000);
        err = rt_task_wait_period(NULL);
        if (err) {
            printf("Error with rt_task_wait_period()\n\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    char name1[10], name2[10], name3[10];
    RTIME period1 = 1000000000;
    RTIME period2 = 2000000000;
    RTIME period3 = 3000000000;

    printf("Starting tasks!\n");
    sprintf(name1, "Task1");
    sprintf(name2, "Task2");
    sprintf(name3, "Task3");

    // creating the tasks
    rt_task_create(&task1, name1, 0, 50, 0);
    rt_task_create(&task2, name2, 0, 50, 0);
    rt_task_create(&task3, name3, 0, 50, 0);

    // starting the tasks
    rt_task_start(&task1, &demo, &period1);
    rt_task_start(&task2, &demo, &period2);
    rt_task_start(&task3, &demo, &period3);

    printf("end program by CTRL-C\n");
    pause();

    return 0;
}