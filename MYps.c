#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

int iteration=0;
bool no_match=0;
int location=0;
bool first_visit=1;


void getallpids(char ** ids);
void uid_match(char * filepath, char * temp, char ** ids);
void comparestrings(char * q, char * id, char ** ids, char * temp);
void is_valid_pid(char * pid,char ** ids);

char * getstatinfo(char * pid, int scenario);
char * statminfo(char * pid);
char * cmdline(char * pid);



struct flags {
    int flag_p;
    int flag_s;
    int flag_U;
    int flag_S;
    int flag_v;
    int flag_c;
    char * pid;
};

void printouts(char * pid, struct flags tags);
void initial_print_formatting(struct flags tags);

int main(int argc, char * argv[]) {

    struct flags cur;

    int opt;
    while ((opt = getopt(argc, argv, "psUSvc")) != -1) {
        switch (opt) {
            case 'p':
                cur.flag_p = 1;
                cur.pid= argv[2];
                break;
            case 's':
                cur.flag_s = 1;
                break;
            case 'U':
                cur.flag_U = 1;
                break;
            case 'S':
                cur.flag_S = 1;
                break;
            case 'v':
                cur.flag_v = 1;
                break;
            case 'c':
                cur.flag_c = 1;
                break;
            default:
                printf("An incorrect/invalid tag was passed\n");
                exit(EXIT_FAILURE);
        }
    }



    char ** ids= (char **)malloc(200 * sizeof(char*)); //very important line. This will hold all pid's we need
    if (cur.flag_p==1){
        is_valid_pid(cur.pid,ids); //means a pid is passed. Check to see if it's valid
    }else{
        getallpids(ids); //check all pid's in the filesystem and compare each uid to program uid
    }

    //location is the size of our char **
    for(int i=0; i<location;i++){
        printouts(ids[i], cur);
    }
    free(ids);
    return 0;
}



void is_valid_pid(char * pid, char ** ids){
    bool isvalid=0;
    struct dirent **namelist;
    int number_contents = scandir("/proc", &namelist, NULL, alphasort);

    for (int i = 0; i < number_contents; i++) {
        char *temp = namelist[i]->d_name;
        //finish the rest of this HERE
        if (strcmp(temp, pid)==0){
            isvalid=1;
        }
    }
    if(isvalid==0){
        puts("A valid pid wasn't passed\n");
        exit(EXIT_FAILURE);
    }else{
        ids[0]=pid;
        location=1;
    }

}




//Ran when no -p tag is passed in
void getallpids(char ** ids) {
    FILE *cur;
    cur = fopen("/proc", "rb");
    if (cur == NULL) {
        puts("something has gone wrong\n");
    }
    struct dirent **namelist;
    int number_contents = scandir("/proc", &namelist, NULL, alphasort);

    for (int i = 0; i < number_contents; i++) {
        char *temp = namelist[i]->d_name;
        int temps = (int) *(temp + 0);
        if (temps >= 48 && temps <= 57) {
            char new_path[500] = "";
            strcpy(new_path, "/proc");
            strcat(new_path, "/");
            strcat(new_path, temp);
            strcat(new_path, "/");
            strcat(new_path, "status");
            uid_match(new_path, temp, ids);
        }
    }
    fclose(cur);
}







//code to compare program uid, to each process uid
void uid_match(char * filepath, char * temp, char ** ids){
    int program_uid=getuid();


    //source: https://stackoverflow.com/questions/8671845/iterating-through-digits-in-integer-in-c
    char arr [16];
    int rc= sprintf(arr, "%d",program_uid);
    if (rc<0){
        puts("error has occured\n");
    }

    char * q=arr;

    FILE * cur;
    cur=fopen(filepath, "rb");
    char *line = NULL;
    ssize_t nread;
    size_t len = 0;
    while ((nread = getline(&line, &len, cur)) != -1) {
        if (strstr(line, "Uid:")!=NULL){
            char id [200]="";
            char * now= strstr(line, ":");
            while(*now !='\0'){
                int ascii=(int) *now;
                if(ascii>=48 && ascii<=57){
                    id[iteration]=*now;
                    iteration++;
                }
                now+=1;
            }
            comparestrings(q,id,ids,temp);
        }
    }
    iteration=0;
    fclose(cur);
}










//actually compare the program uid and process uid
void comparestrings(char * q, char * id, char ** ids, char * temp){
    int number=0;
    while (*(q+number)!='\0'){
        if (!(*(q+number)==*(id+number))){
            no_match=1;
        }
        number+=1;
    }
    if (no_match==0){
        ids[location]= temp;
        location+=1;
    }
    no_match=0;
}


void printouts(char * pid, struct flags tags){
    struct info{
        char * sinfo;
        char * Uinfo;
        char * Sinfo;
        char * vinfo;
        char * cinfo;
    };
    struct info print;

    print.sinfo=getstatinfo(pid,1);
    print.Uinfo=getstatinfo(pid,2);
    print.Sinfo=getstatinfo(pid,3);
    print.vinfo=statminfo(pid);
    print.cinfo= cmdline(pid);

    if (first_visit==1){
        initial_print_formatting(tags);
        first_visit=0;
    }
    printf("%s\t\t",pid);

    if(tags.flag_s==1){
        printf("%s\t\t", print.sinfo);
    }
    if(tags.flag_U!=1){
        printf("%s\t\t", print.Uinfo);
    }
    if(tags.flag_S==1){
        printf("%s\t\t", print.Sinfo);
    }

    if(tags.flag_v==1){
        printf("%s\t\t", print.vinfo);
    }

    if(tags.flag_c!=1){
        printf("%s\t\t", print.cinfo);
    }

    puts("\n");
}



//https://kb.novaordis.com/index.php//proc/pid/stat
char * getstatinfo(char * pid, int scenario){
    char path [100]= "/proc/";
    strcat(path,pid);
    strcat(path,"/stat");
    FILE * temp;
    temp = fopen(path, "rb");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, temp);
    int iterations= 0;
    char * token=strtok(line," ");
    while (token!=NULL){
        if (iterations==2 && scenario==1){
            fclose(temp);
            return token;
        }
        if(iterations==13 && scenario==2){
            fclose(temp);
            return token;
        }
        if(iterations==14 && scenario==3){
            fclose(temp);
            return token;
        }
        token=strtok(NULL, " ");
        iterations+=1;
    }
    fclose(temp);
    return NULL;

}


//https://stackoverflow.com/questions/60644373/how-do-i-decode-the-output-of-proc-statm
char * statminfo(char * pid){
    char  * path = (char *) malloc (100 * sizeof(char *));
    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/statm");

    FILE * cur= fopen(path, "rb");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, cur);
    char * token= strtok(line, " ");
    fclose(cur);
    free(path);
    return token;
}



//www.c-for-dummies.com/blog/?p=1112
char * cmdline(char * pid){
    char *  path = (char *) malloc(50 * sizeof(char *));
    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/cmdline");
    char * new=path;

    FILE * cur = fopen(new, "rb");

    char *line = NULL;
    size_t len = 0;
    size_t characters;
    characters=getline(&line, &len, cur);


    //source: www.c-for-dummies.com/blog/?p=1112
    // if -1, that means it just contains "/0/0"
    if (characters==-1){
        return " ";
    }else{
        return line;
    }
}

//print out heading
void initial_print_formatting(struct flags tags){
    printf("pid\t\t");
    if (tags.flag_s==1){
        printf("State\t\t");
    }
    if (tags.flag_U!=1){
        printf("utime\t\t");
    }
    if(tags.flag_S==1){
        printf("stime\t\t");
    }
    if(tags.flag_v==1){
        printf("virtual_mem\t\t");
    }
    if(tags.flag_c!=1){
        printf("cmdline\t\t");
    }
    printf("\n");
}