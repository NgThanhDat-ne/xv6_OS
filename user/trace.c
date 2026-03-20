#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "Usage: trace mask command\n");
    exit(1);
  }

  int mask = atoi(argv[1]);

  // enable tracing
  trace(mask);

  // run command
  exec(argv[2], &argv[2]);

  // only reached if exec fails
  fprintf(2, "exec %s failed\n", argv[2]);
  exit(1);
}