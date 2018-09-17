#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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

    printf("Start task!\n");
    sprintf(str, "Task A");

    // create the task
    rt_task_create(&hello_task, str, 0, 50, 0);
    rt_task_start(&hello_task, &helloWorld, 0);

    return 0;
}

