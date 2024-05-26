#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int p1[2], p2[2];

  pipe(p1);
  pipe(p2);
  if(fork() == 0){
    char c;
    read(p1[0], &c, 1);
    close(p1[0]);
    fprintf(1, "%d: received ping\n", getpid());
    write(p2[1], &c, 1);
    close(p2[1]);
    exit(0);
  }else{
    char c = 'a';
    write(p1[1], &c, 1);
    close(p1[1]);
    read(p2[0], &c, 1);
    close(p2[0]);
    fprintf(1, "%d: received pong\n", getpid());
    exit(0);
  }
}