#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
new_proc(int p[2])
{
  int n, prime;

  close(p[1]);  // close the input of p
  read(p[0], &prime, 4);  // get the first number from pipe, it must be a prime
  fprintf(1, "prime %d\n", prime);

  if (read(p[0], &n, 4) != 4){  // pipe is empty
    return 0;
  }

  int new_pipe[2];
  pipe(new_pipe);

  if(fork() == 0){  // in child process
    new_proc(new_pipe);
  } else {  // parent
    close(new_pipe[0]);  // the input isn't used by parent
    // read the rest nums, if can't be devide by prime, send it to child process
    if (n % prime) write(new_pipe[1], &n, 4);
    while(read(p[0], &n, 4) == 4){
      if (n % prime){
        write(new_pipe[1], &n, 4);
      }
    }
    close(p[0]);
    close(new_pipe[1]);
    wait(0);  // wait for child
  }
  return 0;
}

int
main(int argc, char* argv[])
{
  int p[2];
  pipe(p);

  if(fork() == 0){  // child
    new_proc(p);
  } else {  // parent
    close(p[0]);  // close input of p
    for(int i=2; i<=35; ++i){
      if (write(p[1], &i, 4) != 4){
        fprintf(2, "Failed to write %d into the pipe\n", i);
        close(p[1]);
        exit(1);
      }
    }
    close(p[1]);  // !! close first, or child wouldn't stop reading
    wait(0);
    exit(0);
  }
  return 0;
}