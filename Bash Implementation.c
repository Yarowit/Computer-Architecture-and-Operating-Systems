#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void dezombify();
int spawn_proc (int in, int out, char ** cmd);

void main(){
    signal(SIGCHLD, dezombify);

    int ctrlDFlag = 0, andFlag = 0;
    size_t size = 100*sizeof(char);
    char * divisor = " \n";

    printf("Welcome to LSH!\n");

    for(;;){
        char * line;
        
        char *** commands = malloc(5*10*30*sizeof(char));
        int i = 0, commandi = 0;

        andFlag = 0;

        printf(">> ");

        printf("Works\n");
        
        ctrlDFlag = getline(&line, &size ,stdin);
        printf("Works\n");
        char * word = strtok(line,divisor);

        printf("Works\n");
        while( word != NULL )
        {
            if(strcmp(word,"&") == 0){
                andFlag = 1;
                break;
            }
            if(strcmp(word,"|") == 0){
                commandi++;
                continue;
            }
            commands[commandi][i] = word;
            word = strtok( NULL, divisor );
        
            i++;
        }

        if (i == 0 && ctrlDFlag != EOF) continue;
        printf("Works\n");
        if (commandi > 0){

            int i;
            pid_t pid;
            int in, fd [2];

            in = 0;

            for (i = 0; i < commandi - 1; ++i){
                pipe (fd);

                spawn_proc (in, fd [1], commands[i]);

                close (fd [1]);

                in = fd [0];
                }

            if (in != 0)
                dup2 (in, 0);

            execvp (commands[i][0], commands[i]);
        }else{

            char ** command = commands[0];

            if(ctrlDFlag == EOF || strcmp(command[0],"exit") == 0 ){
                printf("exited\n");
                break;
                exit(0);
            }else if (strcmp(command[0],"cd") == 0){
                if (chdir(command[1]) != 0)
                    perror("");            
            }else{
                int pid = fork();
                if(pid == 0){
                    execvp(command[0],command);
                    printf("Incorrect command\n");
                    exit(0); 
                }else if(andFlag == 0){
                    printf("Waiting\n");
                    
                }
            }
        }
    }
}

void dezombify(){
    waitpid(0, NULL, WNOHANG);
}


int spawn_proc (int in, int out, char ** cmd)
{
  pid_t pid;

  if ((pid = fork ()) == 0)
    {
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }

      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }

      return execvp (cmd[0], cmd);
    }

  return pid;
}