#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>

// This is the maximum number of arguments your shell should handle for one command
#define MAX_ARGS 128

int main(int argc, char** argv) {
  char* line = NULL;     // Pointer that will hold the line we read in
  size_t line_size = 0;  // The number of bytes available in line

  // Loop forever
  while (true) {
    // Print the shell prompt
    printf("$ ");

    // Get a line of stdin, storing the string pointer in line
    if (getline(&line, &line_size, stdin) == -1) {
      if (errno == EINVAL) {
        perror("Unable to read command line");
        exit(2);
      } else {
        // Must have been end of file (ctrl+D)
        printf("\nShutting down...\n");

        // Exit the infinite loop
        break;
      }
    }

    // store input to exec
    char* ptr = line;
    char* arg[MAX_ARGS];
    char* file = strtok_r(line, "\n ", &ptr);
    int i = 0;

    while (arg[i] != NULL) {
      arg[i] = strtok_r(NULL, " ", &ptr);
      i++;
    }

    if (strcmp(file, "cd") == 0) {
      chdir(arg[1]);
    } else if (strcmp(file, "") == 0) {
      continue;
    } else if (strcmp(file, "exit") == 0 || strcmp(file, "quit") == 0) {
      break;
    } else {
      pid_t child_id = fork();
      if (child_id == 0) {
        execvp(file, arg);
        perror("exec failed");
        exit(EXIT_FAILURE);
      } else if (child_id > 0) {
        int status;
        wait(&status);
        printf("[%s exited with status %d]\n", file, WEXITSTATUS(status));
      } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
      }
    }
  }

  // If we read in at least one line, free this space
  if (line != NULL) {
    free(line);
  }

  return 0;
}
