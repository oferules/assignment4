#include "types.h"
#include "stat.h"
#include "user.h"


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

int
main(int argc, char *argv[])
{
    
    int i, nameOn=0 , sizeOn=0, typeOn=0, tagOn=0, followOn=0;
    char *path, *name, tagKey[10], tagValue[30]; 
    char type;
    int size;
    
    if (argc < 2 || argv[1][0] == '-'){
        printf(1,"Invalid use of find\n");
        exit();
    }

    path= argv[1];

    
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
            if (i >= argc || argv[i][0] == '-'){
                printf(1,"parameter size without value\n");
                exit();
            }
            size = atoi(argv[i]);
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
    
    printf(1, "path: %s\n", path);
    if (followOn) printf(1, "follow\n");
    if (nameOn) printf(1, "name: %s\n", name);
    if (sizeOn) printf(1, "size: %d\n", size);
    if (typeOn) printf(1, "type: %c\n", type);
    if (tagOn) printf(1, "tag key: %s, tag value: %s\n", tagKey, tagValue);
       
   exit();
}
