#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/sem.h>
#include <alchemy/task.h>

#define SEM_INIT 1
#define SEM_MODE S_PRIO

RT_TASK task1, task2, task3, task4, task5;

RT_SEM s1;

void demo(void *arg) {

    int num = * (int *) arg;

    rt_sem_p(&s1, TM_INFINITE);
    RT_TASK_INFO curtaskinfo;
    rt_task_inquire(NULL,&curtaskinfo);
    rt_printf("Task name: %s and ", curtaskinfo.name);
    rt_printf("prio: %d and ", curtaskinfo.prio);
    rt_printf("number: %d\n", num);
    rt_sem_v(&s1);
    rt_task_sleep(rt_timer_ns2ticks(1000000));


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

    // creating the semaphore and tasks
    rt_sem_create(&s1, "sem1", SEM_INIT, SEM_MODE);
    rt_task_create(&task1, name1, 0, 80, 0);
    rt_task_create(&task2, name2, 0, 55, 0);
    rt_task_create(&task3, name3, 0, 60, 0);
    rt_task_create(&task4, name4, 0, 65, 0);
    rt_task_create(&task5, name5, 0, 70, 0);

    rt_sem_p(&s1, TM_INFINITE);
    rt_task_start(&task1, &demo, &index[0]);
    rt_task_start(&task2, &demo, &index[1]);
    rt_task_start(&task3, &demo, &index[2]);
    rt_task_start(&task4, &demo, &index[3]);
    rt_task_start(&task5, &demo, &index[4]);
    rt_sem_v(&s1);

    return 0;
}