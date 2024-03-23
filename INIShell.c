#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char input[1024];
    while (1) {
        printf("user@linux:~$ ");
        fgets(input, sizeof(input), stdin);
        
        // Strip the newline character
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        if (strncmp(input, "cd", 2) == 0) {
            // Handle cd command
            char *path = input + 3; // Skip "cd " part
            if (chdir(path) == -1) {
                perror("chdir");
            }
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            char *args[32]; // Assuming a maximum of 32 arguments
            char *token = strtok(input, " ");
            int i = 0;
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL; // Null-terminate the argument list
            
            execvp(args[0], args);
            
            // If execvp returns, it means there was an error
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        }
    }
    return 0;
}
