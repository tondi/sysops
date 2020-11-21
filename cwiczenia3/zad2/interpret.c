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
    char* line;
    int length;

    while (getline(&line, &length, file) != -1) {
        int args_i = 0;
        int child_process_status;

        char *buffer = strtok (line, " \n\t");

        // printf("%c", buffer[0]);

        while (buffer != NULL && args_i < MAX_ARGS){
            args[args_i] = buffer;
            args_i++;
            buffer = strtok (NULL, " \n\t");
        }
        args[args_i] = NULL;

        // for(int i = 0; i < args_i; i++) {
        //     for(int j = 0; j < 5; j++) {
        //         printf("%c", args[i][j]);

        //     }
        // }
        // printf("\n");

        int pid = fork();

        if (pid == 0) {
            int result = execvp(args[0], args);
            if (result < 0) {
                printf("ERROR: command %s resulted in an error\n", args[0]);
                exit(-1);
            }
        }
        wait(&child_process_status);

        if (child_process_status < 0) {
            printf("ERROR: child process status returned an error. Command: %s", args[0]);
            return -1;
        }
    }

    return 0;
}
