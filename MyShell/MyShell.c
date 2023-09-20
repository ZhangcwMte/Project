#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

char cmd[1024] = {0};
char* _argv[50];

int main()
{
    while(1)
    {
        printf("[root@localhost MyShell]$ ");
        fflush(stdout);

        if(fgets(cmd , sizeof cmd , stdin) == NULL)
            continue;
        cmd[strlen(cmd) - 1] = '\0';

        _argv[0] = strtok(cmd , " ");
        int index = 1;
        if(strcmp(_argv[0] , "ls") == 0)
            _argv[index++] = "--color=auto";

        while(_argv[index++] = strtok(NULL , " "));
        
        if(strcmp(_argv[0] , "cd") == 0)
        {
            if(_argv[1] != NULL)
                chdir(_argv[1]);
            continue;
        }

        pid_t id = fork();
        if(id < 0)
        {
            exit(-1);
        }
        else if(id == 0)
        {
            execvp(_argv[0] , _argv);
            exit(1);
        }

        int status = 0;
        pid_t ret = waitpid(id , &status , 0);
        if(ret > 0)
            printf("return:%d\n" , (status >> 8) & 0xFF);
        else 
            perror("waitpid:");
    }
}
