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

  int fd1 = open("README", O_RDWR);
  const char * key1="key1";
  // add 1 tag
  printf(1, "add first tag\n");
  ftag(fd1,key1,"value1");
  
  // get 1 tag
  char value[30];
  gettag(fd1, key1, value);
  printf(1, "get first tag value: \"%s\"\n",value);
  
  // remove 1 tag and try to get its value
  char value1[30];
  printf(1, "remove first tag\n");
  funtag(fd1,key1);
  printf(1, "try to get first tag value: \"%s\"\n",value1);
  
  // add 2 tag
  const char * key2="key2";
  printf(1, "add second tag\n");
  ftag(fd1, key2,"value2");
  
  // add 3 tag
  const char * key3="key3";
  printf(1, "add third tag\n");
  ftag(fd1, key3,"value3");
  
  // get 2 tag
  char value2[30];
  gettag(fd1, key2, value2);
  printf(1, "get second tag value: \"%s\"\n",value2);
  
  // get 3 tag
  char value3[30];
  gettag(fd1, key3, value3);
  printf(1, "get third tag value: \"%s\"\n",value3);
  
  // add 4 tag
  const char * key4="key4";
  printf(1, "add fourth tag\n");
  ftag(fd1, key4,"value4");
  
  // remove 3 tag
  printf(1, "remove third tag\n");
  funtag(fd1,key3);
  
  // get 4 tag
  char value4[30];
  gettag(fd1, key4, value4);
  printf(1, "get fourh tag value: \"%s\"\n",value4);
  
  // remove 2 tag
  printf(1, "remove second tag\n");
  funtag(fd1,key2);
  
  // get 4 tag
  char value5[30];
  gettag(fd1, key4, value5);
  printf(1, "get fourh tag value: \"%s\"\n",value5);
  
  
  // add 5 tag
  const char * key5="key5";
  printf(1, "add fifth tag\n");
  ftag(fd1, key5,"value5");
  
  // change value of 4 tag
  printf(1, "change value of fourth tag\n");
  ftag(fd1, key4,"value4changed");
  
  // get 4 tag
  char value6[30];
  gettag(fd1, key4, value6);
  printf(1, "get fourh tag value: \"%s\"\n",value6);
  
  // get 5 tag
  char value7[30];
  gettag(fd1, key5, value7);
  printf(1, "get fifth tag value: \"%s\"\n",value7);
  
  // close and open fd and try to get 4,5 tags
  printf(1, "close and open fd\n");
  close(fd1);
  fd1 = open("README", O_RDWR);
  
  // get 4 tag
  char value8[30];
  gettag(fd1, key4, value8);
  printf(1, "get fourth tag value: \"%s\"\n",value8);
  
  // get 5 tag
  char value9[30];
  gettag(fd1, key5, value9);
  printf(1, "get fifth tag value: \"%s\"\n",value9);
  
  close(fd1);
  exit();
}