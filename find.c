#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


int nameOn=0 , sizeOn=0, typeOn=0, tagOn=0, followOn=0, moreThen=0, lessThen=0;
char *root_path, *name, tagKey[10], tagValue[30]; 
char type;
int size;

void 
parseTag(const char* arg, char *tagKey, char *tagValue){
    int i=0;
    while (arg[i] != '='){
        if (arg[i] == '\0'){
            printf(1,"Invalid use of type (no =)\n");
            exit();
        }
        i++;
    }
    
    if (i==0){
        printf(1,"Invalid use of type (no key)\n");
        exit();
    }
    
    memmove((void*)tagKey, (void*)arg, i);
    memmove( (void*)(tagKey+i), (void*)"\0", 1);
    
    int var_len= 0;
    i++;
    int value_start=i;
    while (arg[i] != '\0'){
        var_len++;
        i++;
    }
    
    if (var_len==0){
        printf(1,"Invalid use of type (no value)\n");
        exit();
    }
    
    memmove((void*)tagValue, (void*)(&arg[value_start]), var_len);
    memmove((void*)(tagValue + var_len), (void*)"\0", 1);
    
}


void
fmtname(char *path, char* curname)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  int i=0;
  while(p[i] != '\0'){
    i++;
  }
  
  memmove(curname, p, i+1);
}

void
find(char *path)
{
  //char buf[512], *p;
  int fd;
  //struct dirent de;
  struct stat st;

  if((fd = open_no_deref(path, 0)) < 0){
    printf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char file_name[100];
  
  fmtname(path, file_name);
  int print = 1;
  
  switch(st.type){
  case T_FILE:
    if (nameOn){
        print = ( strcmp(file_name, name) == 0 );
    }
    
    if (sizeOn){
        if (moreThen)
            print = (st.size > size);
        else if(lessThen)
            print = (st.size < size);
        else
            print = (st.size == size);
    }
    
    if (typeOn){
        print = (type == 'f');
    }
    
    if (tagOn){
        print = 1;
    }
    
    if(print){
        printf(1, "%s\n", path);
    }
    break;
      
  case T_SYMLINK:

    break;

  case T_DIR:
    /*
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat_no_deref(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    */
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
    int i;
    if (argc < 2 || argv[1][0] == '-'){
        printf(1,"Invalid use of find\n");
        exit();
    }

    root_path= argv[1];

    
    /// parse arguments
    for (i=2; i < argc ; i++){
        if (strcmp(argv[i], "-follow") == 0){
            followOn=1;
        }
        else if (strcmp(argv[i], "-name") == 0){
            nameOn=1;
            
            i++;
            if (i >= argc || argv[i][0] == '-'){
                printf(1,"parameter name without value\n");
                exit();
            }
            name = argv[i];
        }
        else if (strcmp(argv[i], "-size") == 0){
            sizeOn=1;
            
            i++;
            if (i >= argc){
                printf(1,"parameter size without value\n");
                exit();
            }
            
            if(argv[i][0] == '+'){
                moreThen=1;
                size = atoi(argv[i] + 1);
            }
            else if(argv[i][0] == '-'){
                lessThen=1;
                size = atoi(argv[i] + 1);
            }
            else{
                int j;
                for (j=0; j< strlen(argv[i]) ; j++){
                    if (argv[i][j] < '0' || argv[i][j] > '9' ) {
                        printf(1,"invalid size: %s\n",argv[i]);
                        exit();
                    }
                        
                }
                size = atoi(argv[i]);
            }
        }
        else if (strcmp(argv[i], "-type") == 0){
            typeOn=1;
            
            i++;
            if (i >= argc || argv[i][0] == '-'){
                printf(1,"parameter type without value\n");
                exit();
            }
            if ( ! (strcmp(argv[i],"d")==0 || strcmp(argv[i],"f")==0  || strcmp(argv[i],"s")==0 ) ){
                printf(1,"Invalid type: %s\n", argv[i]);
                exit();
            }
            type= argv[i][0];
        }
        else if (strcmp(argv[i], "-tag") == 0){
            tagOn=1;
            
            i++;
            if (i >= argc || argv[i][0] == '-'){
                printf(1,"parameter tag without value\n");
                exit();
            }
            parseTag(argv[i], tagKey, tagValue);
            
        }
        else{
            printf(1,"Invalid parameter: %s\n" , argv[i]);
            exit();
        }
    }
    
    /*
    printf(1, "path: %s\n", root_path);
    if (followOn) printf(1, "follow\n");
    if (nameOn) printf(1, "name: %s\n", name);
    if (sizeOn) printf(1, "size: %s%s %d\n", moreThen? "more then" : "", lessThen? "less then" : "", size);
    if (typeOn) printf(1, "type: %c\n", type);
    if (tagOn) printf(1, "tag key: %s, tag value: %s\n", tagKey, tagValue);
    */
    
    find(root_path);
    exit();
}
