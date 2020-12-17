/**
 * 计算context switch的开销
 * 优化：还需要减去read write的系统调用开销。
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define __USE_GNU
#include <sched.h>

// send recv nums
const int num = 1000*10;

int child_process(int rw_pipe1[2],int rw_pipe2[2])
{
    struct timeval start, end;
    gettimeofday(&start,NULL);
    
    for (int i = 0; i < num; i++) {
        //recv
        read(rw_pipe1[0],NULL,0);
        // send
        write(rw_pipe2[1],NULL,0);
    }
    
    gettimeofday(&end,NULL);
    double time_cost = (double)((end.tv_sec - start.tv_sec)*1000*1000 +
                                (end.tv_usec - start.tv_usec));
    printf("context switch cost: %f us\n",time_cost/(2*num));
}

int parent_process(int rw_pipe1[2],int rw_pipe2[2])
{
    for (int i = 0; i < num; i++) {
        // send
        write(rw_pipe1[1],NULL,0);
        //recv
        read(rw_pipe2[0],NULL,0);
    }
}


int main()
{
    int rw_pipe1[2];
    int rw_pipe2[2];
    pipe(rw_pipe1);
    pipe(rw_pipe2);
    // bind two processes on the same cpu
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0,&mask);
    
    // 使用父进程作为第一个发送端
    if (fork() == 0) {
        // child
        if(sched_setaffinity(0,sizeof(cpu_set_t),&mask)==-1){
            perror("set affinity failed");
            exit(-1);
        }
        child_process(rw_pipe1,rw_pipe2);
    } else {
        // parent
        if(sched_setaffinity(0,sizeof(cpu_set_t),&mask)==-1){
            perror("set affinity failed");
            exit(-1);
        }
        parent_process(rw_pipe1,rw_pipe2);
        wait(NULL);
        // close pipes
        close(rw_pipe1[0]); close(rw_pipe1[1]);
        close(rw_pipe2[0]); close(rw_pipe2[1]);
    }
    return 0;
}
