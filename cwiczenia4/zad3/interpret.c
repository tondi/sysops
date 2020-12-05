#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 16

int main(int argc, char *argv[]) {


    if (argc != 2) {
        printf("correct usage: interpret <filename>");
        exit(-1);
    }

    FILE *file;
    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("file not found");
        exit(-1);
    }

    char *args[MAX_ARGS + 1];
    args[MAX_ARGS] = NULL;
    
    char *p_args[MAX_ARGS + 1];
    p_args[MAX_ARGS] = NULL;
    
    char* line;
    size_t length;
    int pipe_fd[2];
    int prev_read_desc;

    int child_process_status;
    int i = 0;

    
    // while(i < 2) {
    //     pid_t pid = fork();
    //     if (pid == 0) {
    //         printf("%s\n", "child");
    //         printf("%s\n", "child");
    //         printf("%s\n", "child");
    //     } else if (pid > 0) {
    //         printf("%s\n", "parent");
    //         printf("%s\n", "parent");
    //         printf("%s\n", "parent");
    //     } else {
    //         printf("%s\n", "error");
    //         printf("%s\n", "error");
    //         printf("%s\n", "error");
    //     }
    //     wait(&child_process_status);
    // }

    while (getline(&line, &length, file) != -1) {
        pipe(pipe_fd);
        int piped = 0;
        int prev_piped = 0;

        int pipe_iterator = 0;
        int child_process_status;

        char *end_str;
        char *pipe_buffer = strtok_r(line, "|", &end_str);

        // printf("%c", buffer[0]);
        
        // separate by pipes
        while (pipe_buffer != NULL && pipe_iterator < MAX_ARGS) { // TODO add MAX_PIPES?
            // puts(pipe_buffer);
            char *args_end_str;
            // printf("%d\n", pipe_iterator);

            // separate by arguments
            char *buffer = strtok_r(pipe_buffer, " \n\t", &args_end_str);
            int arg_iterator = 0;

            while (buffer != NULL && arg_iterator < MAX_ARGS){
                args[arg_iterator] = buffer;
                // puts(args[arg_iterator]);
                arg_iterator++;
                buffer = strtok_r(NULL, " \n\t", &args_end_str);
            }
            // puts("\n");

            // piped = 1; // TODO
            // prev_piped = 1;

            // run first program
            // pid
            // pipe it to next one
            // fflush( stdout );

            pid_t pid = fork();

            printf("%d\n", pid);

            if (pid == 0) { // child
                // fflush(stdout);
                printf("I am %d, from iteration %d\n",getpid(), pid);

                puts("child");
                if (piped) dup2(pipe_fd[1], STDOUT_FILENO);
                if (prev_piped) dup2(prev_read_desc, STDIN_FILENO);
                
                int result = execvp(args[0], args);
                if (result < 0) {
                    printf("ERROR: command %s resulted in an error\n", args[0]);
                    exit(-1);
                }
            } else if(pid > 0) {
                puts("parent");
                exit(1);
            } else {
                puts("error");
            }
            wait(&child_process_status);

            // if (child_process_status < 0) {
            //     printf("ERROR: child process status returned an error. Command: %s", args[0]);
            //     return -1;
            // }

            // prev_piped = piped;
            // if (prev_piped) prev_read_desc = pipe_fd[0];
            // close(pipe_fd[1]);

            pipe_iterator++;

            pipe_buffer = strtok_r(NULL, "|", &end_str);
        }
        
        puts("\n");
        // printf("%d", pipe_iterator);

        // p_args[pipe_iterator] = NULL;
        
    }
    

    return 0;
}
