#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    struct timeval tv1,tv2;
    struct timeval tmp_tv;
    int fd;
    const int loop_nums = 1000*1000;
    
    fd = open("/tmp/tmp.txt",O_WRONLY | O_CREAT);
    if(fd == -1){
        perror("open test file failed");
        return -1;
    }
    
    gettimeofday(&tv1,NULL);
    for(int i = 0;i<loop_nums;i++){
        read(fd,NULL,0);
    }
    gettimeofday(&tv2,NULL);
    double time_cost = (double)((tv2.tv_sec - tv1.tv_sec)*1000*1000+(tv2.tv_usec - tv1.tv_usec));
    printf("time cost %f us\n",time_cost);
    printf("system call cost %f us\n",time_cost/(loop_nums));
    return 0;
}
