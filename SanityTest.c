#include "types.h"
#include "stat.h"
#include "user.h"

#define BSIZE 512
#define MAX_DIRECT 6144
#define MAX_SINGLE_INDIRECT 65536
#define COMPLETE_TO_1MB (1048576 - (MAX_DIRECT + MAX_SINGLE_INDIRECT))
#define O_CREATE 0x200
#define O_RDWR 0x002

int
main(int argc, char *argv[])
{
  
  int i, fd;
  char buf[BSIZE];

  memset(buf , 5 , BSIZE / 4 );

  fd = open("sanity", O_CREATE|O_RDWR);
  
  if(fd < 0){
    printf(1, "error creating file!\n");
    exit();
  }
  


  for(i = 0; i < MAX_DIRECT / BSIZE ; i ++ ){
      
    if(write(fd, buf, BSIZE) != BSIZE){
      printf(1, "error: write to direct fail\n");
      exit();
    }
  }
  
  printf(1, "Finished writing 6KB (direct)\n");
  
    for(i = 0; i < MAX_SINGLE_INDIRECT / BSIZE ; i ++ ){
      
        if(write(fd, buf, BSIZE) != BSIZE){
        printf(1, "error: write to single indirect fail\n");
        exit();
        }
    }
  
    printf(1, "Finished writing 70KB (single indirect)\n");
  
    for(i = 0; i < COMPLETE_TO_1MB / BSIZE ; i ++ ){
      
        if(write(fd, buf, BSIZE) != BSIZE){
        
        printf(1, "error: write to double indirect fail %d\n", i);
        exit();
        }
    }
    close(fd);
  
    printf(1, "Finished writing 1MB\n");
  
   exit();
}
