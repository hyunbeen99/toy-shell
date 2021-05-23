#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//Parameter
#define MAX_LEN_LINE    100
#define PATH_MAX        100
#define LEN_HOSTNAME    30

//command line color set
#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW 	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define DEFAULT	"\x1B[0m"

void getshellprompt();
void built_command(char *s);
int change_dir(char *s);

int change_dir(char *s){
	int result = chdir(s);
	return 1;
}

int main(void)
{
    char command[MAX_LEN_LINE];
    char *arg1;
    char *args[] = {command, arg1, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    while (true) {
		char *s;
		int len;

		getshellprompt();

		s = fgets(command, MAX_LEN_LINE, stdin);
		
		if(!strcmp(s, "clear\n") || !strcmp(s, "cd\n")){
			built_command(s);
			continue;
		}
		else{
			built_command(s);
		}

		len = strlen(command);
		printf("%d\n", len);

		if (command[len - 1] == '\n') {
			command[len - 1] = '\0'; 
		}

		arg1 = strtok(command, " ");

		printf("[%s]\n", command);


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

			// Print Current directory path => 'pwd'
			if (strcmp(arg1,"pwd") == 0){
				arg1 = strtok(NULL, " ");
				args[0] = "/bin/pwd";
				args[1] = arg1;
			}
			// Print list files if directory => 'ls'
			else if (strcmp(command,"ls") == 0){
				arg1 = strtok(NULL, " ");
				args[0] = "/bin/ls";
				args[1] = arg1;
			}
			else{
				printf("%s: command not found\n", arg1);
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

void getshellprompt(void){

	char hostname[LEN_HOSTNAME + 1];	
	char cwd[PATH_MAX];	

	memset(hostname, 0x00, sizeof(hostname));
	gethostname(hostname, LEN_HOSTNAME);
	getcwd(cwd, sizeof(cwd));

	printf(YELLOW);
	printf("%s@%s:", getpwuid(getuid())->pw_name, hostname);
	printf(BLUE);
	printf("%s$ ",cwd);
	printf(DEFAULT);

}

void built_command(char *s){
	char changedir[PATH_MAX];

	if (s == NULL) {
		fprintf(stderr, "fgets failed\n");
		exit(1);
	}
	// Exit => 'exit'
	if (strcmp(s,"exit\n") == 0){
		printf(RED "GOOD BYE\n" DEFAULT);
		exit(0);
	}
	// Clear => 'clear'
	if (strcmp(s,"clear\n") == 0){
		printf("\e[1;1H\e[2J");
	}
	
	// Change directory => 'cd'
	if(strcmp(s, "cd\n")==0){
		chdir(getenv("HOME"));
	} 
}
