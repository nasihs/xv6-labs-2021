#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
new_proc(int p[2])
{
  int n, prime;

  read(p[0], &prime, 4);
  fprintf(1, "prime: %d\n", prime);

  if (read(p[0], &n, 4) != 4){
    return 0;
  }

  int new_pipe[2];
  pipe(new_pipe);

  if(fork() == 0){
    new_proc(new_pipe);
  } else {
    close(new_pipe[0]);
    if (n % prime) write(new_pipe[1], &n, 4);
    while(read(p[0], &n, 4) == 4){
      if (n % prime){
        write(new_pipe[1], &n, 4);
      }
    }
    close(p[0]);
    close(new_pipe[1]);
    wait(0);
  }
  return 0;
}

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);

  if(fork() == 0){
    new_proc(p);
  } else {
    close(p[0]);
    for(int i=2; i<=35; ++i){
      if (write(p[1], &i, 4) != 4){
        fprintf(2, "Failed to write\n");
        exit(1);
      }
    }
  }
  return 0;
}