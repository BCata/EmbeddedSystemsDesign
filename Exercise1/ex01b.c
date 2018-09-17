#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <alchemy/task.h>

RT_TASK hello_task;

// this function will be executed by the task
void helloWorld(void* arg) {
    RT_TASK_INFO curtaskinfo;

    printf("Hello world!\n");

    // inquire current task (???)
    rt_task_inquire(NULL, &curtaskinfo);

    // print task name
    printf("Task name: %s\n", curtaskinfo.name);

}

int main(int argc, char* argv[]) {
    char str[10];
    int retval;

    printf("Start task!\n");
    sprintf(str, "Task B");

    // create the task
    rt_task_create(&hello_task, str, 0, 50, 0);
    retval = rt_task_start(NULL, &helloWorld, 0);

    if (retval < 0) {
        rt_printf("Sending error %d: %s\n", -retval, strerror(-retval));
    } else {
        rt_printf("Task B executed succesfully\n");
    }

    return 0;
}

