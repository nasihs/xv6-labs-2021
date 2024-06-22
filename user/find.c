#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void
find(char* path, char* target)
{
  int fd;
  struct dirent de;
  struct stat st;

  char buf[32], *p;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    close(fd);
    break;
  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    *p = '\0';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || 
        (strcmp(".", de.name) == 0) || 
        (strcmp("..", de.name) == 0))
          continue;

      strcpy(p, de.name);  // !!! use p instead of buf to clear last de.name

      if(stat(buf, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", buf);
        continue;
      }

      if(st.type == T_FILE || st.type == T_DEVICE){
        // printf("de.name=%s, target=%s\n", de.name, target);
        if(strcmp(de.name, target) == 0){
          fprintf(1, "%s\n", buf);
        }
      }else if(st.type == T_DIR){
        // printf("to find %s in dir: %s\n", target, buf);
        find(buf, target);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char* argv[])
{
  if(argc != 3){
    fprintf(1, "Usage: find <path> <target>\n");
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}