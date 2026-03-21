#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct procinfo {
  int pid;
  int ppid;
  int state;
  uint64 sz;
  char name[16];
};


char *states[] = {
  "UNUSED", "USED", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"
};

int main(int argc, char *argv[]) {
  struct procinfo info;
  int pid;

  if(argc > 1)
    pid = atoi(argv[1]);   // use argument if provided
  else
    pid = getpid();        // default to self

  if(procinfo(pid, &info) == 0) {
    printf("Name:   %s\n",  info.name);
    printf("PID:    %d\n",  info.pid);
    printf("PPID:   %d\n",  info.ppid);
    printf("State:  %s\n",  states[info.state]);
    printf("Memory: %d bytes\n", (int)info.sz);
  } else {
    printf("procinfo failed\n");
  }
  exit(0);
}