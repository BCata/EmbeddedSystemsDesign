#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/mutex.h>
#include <alchemy/timer.h>

#define NTASKS 3
#define LOW_PRIORITY 49
#define MEDIUM_PRIORITY 50
#define HIGH_PRIORITY 51


RT_TASK demo_task[NTASKS];
RT_MUTEX mysync;
  
void prioLow(void* arg){
	int i = 0;
	rt_printf(">>> Low started\n");
	while(i < 3){
		
		rt_printf("Low priority task tries to lock mutex\n");
		rt_mutex_acquire(&mysync, TM_INFINITE);
		rt_printf("Low priority task locks mutex\n");
		rt_task_sleep(10000);
		rt_printf("Low priority task unlocks mutex\n");
		rt_mutex_release(&mysync);
		
		i++;
	}
	
	rt_printf("....................................Low priority task ends\n");
}

void prioMedium(void* arg){
	int i = 0;
	rt_printf(">>> Medium started\n");
	rt_task_sleep(1000);		// Without this sleep, highPrio task does not have the chance to start and the experiment cannot be reproduced
	
	while(i < 10000000){
		if(i % 1000000 == 0){
			rt_printf("Medium task running\n");
		}
		i++;
	}
	
	rt_printf("....................................Medium priority task ends\n");
}	

void prioHigh(void* arg){
	int i = 0;
	rt_printf(">>> High started\n");
	while(i < 3){
		
		rt_printf("High priority task tries to lock mutex\n");
		rt_mutex_acquire(&mysync, TM_INFINITE);
		rt_printf("High priority task locks mutex\n");
		rt_printf("High priority task unlocks mutex\n");
		rt_mutex_release(&mysync);

		i++;
	}
	
	rt_printf("....................................High priority task ends\n");

}

void startup() {
	int i;
	// mutex to sync task startup on
	rt_mutex_create(&mysync,"MyMutex");
	
	rt_task_set_mode(0,XNRRB,NULL);
	
	rt_task_create(&demo_task[0], "lowTask", 0, LOW_PRIORITY, 0);
	rt_task_create(&demo_task[1], "mediumTask", 0, MEDIUM_PRIORITY, 0);
	rt_task_create(&demo_task[2], "highTask", 0, HIGH_PRIORITY, 0);
	
	rt_task_start(&demo_task[0], &prioLow, NULL);
	rt_task_start(&demo_task[1], &prioMedium, NULL);
	rt_task_start(&demo_task[2], &prioHigh, NULL);
}

int main(int argc, char* argv[])
{
  startup();
  printf("\nType CTRL-C to end this program\n\n" );
  pause();
}