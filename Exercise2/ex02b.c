#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>

RT_TASK task1, task2, task3, task4, task5;

void demo(void *arg) {

    int num = * (int *) arg;

    RT_TASK_INFO curtaskinfo;
    rt_task_inquire(NULL,&curtaskinfo);
    rt_printf("Task name: %s and ", curtaskinfo.name);
    rt_printf("index number: %d\n", num);
}

int main(int argc, char* argv[]) {
    char name1[10], name2[10], name3[10], name4[10], name5[10];
    int index[5];

    int i = 0;

    for (i=0; i<5; i++) {
        index[i] = rand()%100;
    }

    printf("Starting tasks!\n");
    sprintf(name1, "Task1");
    sprintf(name2, "Task2");
    sprintf(name3, "Task3");
    sprintf(name4, "Task4");
    sprintf(name5, "Task5");

    // creating the tasks
    rt_task_create(&task1, name1, 0, 50, 0);
    rt_task_create(&task2, name2, 0, 50, 0);
    rt_task_create(&task3, name3, 0, 50, 0);
    rt_task_create(&task4, name4, 0, 50, 0);
    rt_task_create(&task5, name5, 0, 50, 0);

    rt_task_start(&task1, &demo, &index[0]);
    rt_task_start(&task2, &demo, &index[1]);
    rt_task_start(&task3, &demo, &index[2]);
    rt_task_start(&task4, &demo, &index[3]);
    rt_task_start(&task5, &demo, &index[4]);

    return 0;
}