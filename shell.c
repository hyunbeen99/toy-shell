#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN_LINE    100
#define PATH_MAX        100

int main(void)
{
    char command[MAX_LEN_LINE];
	char cwd[PATH_MAX];
    char *args[] = {command, NULL};
	//char myalias = {"exit", "pwd", "ls"};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
        char *s;
        int len;
        
        printf("MyShell $ ");
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
		// Exit => 'exit'
		if (strcmp(s,"exit\n") == 0){
			printf("GOOD BYE\n");
			exit(0);
		}
		// Print Current directory path => 'pwd'
		if (strcmp(s,"pwd\n") == 0){
			if (getcwd(cwd, sizeof(cwd)) != NULL) {
				printf("DIRECTORY PATH : %s\n", cwd);
			}else{
				perror("cwd error");
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
				return EXIT_FAILURE;
			}
		}
        
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
