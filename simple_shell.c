#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char *input = NULL;
    size_t input_size = 0;

    while (1)
    {
        printf("$ "); // Display the shell prompt
        if (getline(&input, &input_size, stdin) == -1)
        {
            if (feof(stdin))
            {
                printf("\n"); // Handle end of file (Ctrl+D)
                free(input);
                exit(EXIT_SUCCESS);
            }
            else
            {
                perror("getline");
                free(input);
                exit(EXIT_FAILURE);
            }
        }

        input[strcspn(input, "\n")] = '\0'; // Remove the newline character

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            exit(EXIT_SUCCESS);
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            free(input);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            if (execve(input, NULL, NULL) == -1)
            {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Parent process
            int status;
            wait(&status);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "Command not found: %s\n", input);
            }
        }
    }

    return 0;
}

