#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  if(argc != 2){
    fprintf(2, "usage: sleep time\n");
    exit(1);
  }

  int time = atoi(argv[1]);

  if(sleep(time) != 0){
    fprintf(2, "sleep: error in sys_call\n");
  }

  exit(0);
}
