#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>

#define ITER 10
#define SEM_INIT 1
#define SEM_MODE S_FIFO  

static RT_TASK  t1;
static RT_TASK  t2;

static RT_SEM s1;

int global = 0;

void taskOne(void *arg)
{
    int i;
    
    for (i=0; i < ITER; i++) {
        rt_sem_p(&s1, TM_INFINITE);
        printf("I am taskOne and global = %d................\n", ++global);
        rt_sem_v(&s1);
        rt_task_sleep(rt_timer_ns2ticks(100000));
    }
}

void taskTwo(void *arg)
{
    int i;
    for (i=0; i < ITER; i++) {
        rt_sem_p(&s1, TM_INFINITE);
        printf("I am taskTwo and global = %d----------------\n", --global);
        rt_sem_v(&s1);
        rt_task_sleep(rt_timer_ns2ticks(100000));
    }
}

int main(int argc, char* argv[]) {
    rt_task_create(&t1, "task1", 0, 1, 0);
    rt_task_create(&t2, "task2", 0, 1, 0);
    rt_sem_create(&s1, "sem1", SEM_INIT, SEM_MODE);
    rt_task_start(&t1, &taskOne, 0);
    rt_task_start(&t2, &taskTwo, 0);
    return 0;
}
