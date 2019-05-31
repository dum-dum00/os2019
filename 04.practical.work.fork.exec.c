#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

int main(){
  int pid = fork();
  if (pid){
    printf("Main process, first child process's id: %d\n",pid);
    wait(NULL);
    printf("Finish executing first child process!\n");
    pid = fork();
    if(pid){
      printf("Main process, second child process's id: %d\n",pid);
      wait(NULL);
      printf("Finish executing second child process!\n");
    }

    else if (pid == 0){
      printf("child process 2 begin run:\n");
      char *arg[]={"/bin/free","-h",NULL};
      execvp(arg[0],arg);
    }

    else printf("error can not fork second child\n");
  }

  else if (pid == 0){
    printf("child process 1 begin run:\n");
    char *arg[]={"/bin/ps","-ef",NULL};
    execvp(arg[0],arg);
  }

  else printf("error can not fork anything\n");
  return 0;
}