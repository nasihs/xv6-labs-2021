#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
xargs(char* cmd, char** args)
{
  if(fork() == 0){
    exec(cmd, args);
  }else{
    wait(0);
  }
}


int
main(int argc, char* argv[])
{
  int i;
  char* cmd = argv[1];
  char* args[MAXARG];
  memset(args, 0, sizeof(args));

  for(i=2; i<argc; ++i){
    args[i-2] = argv[i];
    printf("aa arg[%d]=%s\n", i, argv[i]);
  }

  char ch;
  char earg[32], *p = earg;
  
  while(read(0, &ch, 1) != 0){
    printf("read ch: %c\n", ch);
    if(ch != '\n' || ch != '\0'){
      *p++ = ch;
    }else{
      printf("to exec\n");
      *p = '\0';
      args[i] = earg;
      printf("bb arg[%d]: %s\n\n", i, earg);
      xargs(cmd, args);
      p = earg;
    }
  }


}

