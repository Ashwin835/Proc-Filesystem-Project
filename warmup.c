#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

void cpuinfo(char *cur_path);
void meminfo(char * cur_path);

int main() {
    char * paths []= {"/proc/cpuinfo","/proc/meminfo"};
    cpuinfo(paths[0]);
    meminfo(paths[1]);
    return 0;
}





void cpuinfo(char *cur_path){
FILE * cur;
    cur= fopen(cur_path, "rb");
    char *line = NULL;
    ssize_t nread;
    size_t len = 0;
    while ((nread = getline(&line, &len, cur)) != -1) {
        if (strstr(line, "processor")!=NULL){
            printf("Processor ");
            char * processor= strstr(line, ":");
            while (*processor !='\0'){
                int ascii= (int) * processor;
                if (ascii>=48 && ascii<=57){
                printf("%c", *processor);
            }
            processor+=1;
        }
        printf(" with a case size of ");
        }

        if (strstr(line, "cache")!=NULL){
            if (strstr(line,"size")!=NULL){
            char * cache= strstr(line, ":");
            while (*cache !='\0'){
                int anotherascii= (int) * cache;
                if (anotherascii>=48 && anotherascii<=57){
                printf("%c", *cache);
            }
            cache+=1;
            }
            printf(" KB\n");
        }
    }
}
fclose(cur);
}









void meminfo(char * cur_path){
    FILE * cur;
    cur= fopen(cur_path, "rb");
    char *line = NULL;
    ssize_t nread;
    size_t len = 0;
    while ((nread = getline(&line, &len, cur)) != -1) {
        if (strstr(line,"MemFree:")!=NULL){
            char * free= strstr(line,"MemFree:");
            printf("Free memory ");
            while(*free != '\0'){
                int ascii= (int) *free;
                if (ascii>=48 && ascii<=57){
                    printf("%c", *free);
                }
                free+=1;
            }
            printf(" kB\n");
        }

        if (strstr(line, "Buffers:")!=NULL){
            char * buffers= strstr(line, "Buffers:");
            printf("Buffers: ");
            while(*buffers!='\0'){
                int anotherascii=(int) *buffers;
                if (anotherascii>=48 && anotherascii<=57){
                    printf("%c", *buffers);
                }
                buffers+=1;
            }
            printf(" kB\n");
        }
    }
    fclose(cur);
}
