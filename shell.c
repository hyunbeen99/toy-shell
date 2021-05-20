#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

//Parameter
#define MAX_LEN_LINE    100
#define PATH_MAX        100

//command line color set
#define RED				"\x1B[31m"
#define GREEN			"\x1B[32m"
#define YELLOW 			"\x1B[33m"
#define BLUE			"\x1B[34m"
#define DEFAULT			"\x1B[0m"

//main function for command
void commandFunc(char *s){

	char cwd[PATH_MAX];	

	if (s == NULL) {
		fprintf(stderr, "fgets failed\n");
		exit(1);
	}
	// Exit => 'exit'
	if (strcmp(s,"exit\n") == 0){
		printf(RED "GOOD BYE\n");
		exit(0);
	}
	// Print Current directory path => 'pwd'
	if (strcmp(s,"pwd\n") == 0){
		if (getcwd(cwd, sizeof(cwd)) != NULL){
			printf("DIRECTORY PATH : %s\n", cwd);
			return;
		}else{
			perror("cwd error");
			return;
		}
	}
	// Print list files if directory => 'ls'
	if (strcmp(s,"ls\n") == 0){
		DIR *d;
		struct dirent *dir;
		if ((d = opendir(".")) != NULL){
			while ((dir = readdir(d)) != NULL){
				printf("%s  ", dir->d_name); 
			}
			printf("\n");
			closedir(d);
		}else {
			perror("can not list directory");
			return;
		}
	}
	
	else{
		printf("Command not found :(\n");
	}

}

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
        char *s;
        int len;
        
        printf(YELLOW "HyunBeen@Shell $ " DEFAULT);
        s = fgets(command, MAX_LEN_LINE, stdin);

		commandFunc(s);
		
        len = strlen(command);
        //printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        pid = fork();
        if (pid < 0) {
            //fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                //fprintf(stderr, "waitpid failed\n");        
            }
            //printf("Child process terminated\n");
            if (WIFEXITED(status)) {
               // printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                //fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
