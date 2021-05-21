#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//Parameter
#define MAX_LEN_LINE    100
#define MAX_LEN_COMMAND_LINE 1000
#define PATH_MAX        100

//command line color set
#define RED			"\x1B[31m"
#define GREEN		"\x1B[32m"
#define YELLOW 		"\x1B[33m"
#define BLUE		"\x1B[34m"
#define DEFAULT		"\x1B[0m"

int main(void)
{
    char command[MAX_LEN_LINE];
    char command_path[MAX_LEN_COMMAND_LINE];
	char cwd[PATH_MAX];	
    char *args[] = {command_path, command , NULL};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
        char *s;
        int len;
        
        printf(YELLOW "HyunBeen@Shell $ " DEFAULT);
        s = fgets(command, MAX_LEN_LINE, stdin);

		if (s == NULL) {
			fprintf(stderr, "fgets failed\n");
			exit(1);
		}
		// Exit => 'exit'
		if (strcmp(s,"exit\n") == 0){
			printf(RED "GOOD BYE\n" DEFAULT);
			exit(0);
		}

        len = strlen(command);
        //printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
			if (execvp(args[1], args) == -1){
				printf("%s: commend not found\n", args[1]);
			}
			// Print Current directory path => 'pwd'
			else if (strcmp(s,"pwd\n") == 0){
				args[0] = "/bin/pwd";
			}
			// Print list files if directory => 'ls'
			else if (strcmp(s,"ls\n") == 0){
				args[0] = "/bin/ls";
			}
			ret= execve(args[0], args, NULL);
			if (ret < 0) {
				fprintf(stderr, "execve failed : %s\n", strerror(errno));   
				return 1;
			}
		}
    }
    return 0;
}
