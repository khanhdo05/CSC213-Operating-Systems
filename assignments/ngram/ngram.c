#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  // Make sure the user provided an N parameter on the command line
  if (argc != 2) {
    fprintf(stderr, "Usage: %s N (N must be >= 1)\n", argv[0]);
    exit(1);
  }

  // Convert the N parameter to an integer
  int N = atoi(argv[1]);

  // Make sure N is >= 1
  if (N < 1) {
    fprintf(stderr, "Invalid N value %d\n", N);
    exit(1);
  }

  // Keep a sliding window of the last N characters read
  char* curr_ngram = malloc((size_t)N);
  if (curr_ngram == NULL) {
    fprintf(stderr, "Failed to allocate array size of %d\n", N);
    exit(1);
  }

  // Hold each character from standard input
  int curr_char;

  // Condition to print the ngram is when the size of the sliding window == N
  // Keep track of the current size of the sliding window
  int curr_size = 0;

  // Read each character of the input until EOF
    while ((curr_char = fgetc(stdin)) != EOF) {
    // Window size less than N so we can add more characters
    if (curr_size < N) {
      curr_ngram[curr_size++] = (char)curr_char;

    // ngram filled
    } else {
      // Shift the characters in the window to the left
      // memmove: https://man7.org/linux/man-pages/man3/memmove.3.html
      // Not the most efficient if N is huge but we'll do with this for now.
      memmove(curr_ngram, curr_ngram + 1, (size_t)(N - 1));

      // Add the new character to the end of the window
      curr_ngram[N - 1] = (char)curr_char;
    }

    // Print the ngram if the window is filled
    if (curr_size == N) {
      // Use fwrite because curr_ngram is not null-terminated.
      fwrite(curr_ngram, 1, (size_t)N, stdout);
      printf("\n");
    }
  }

  // Clean up
  free(curr_ngram);

  return 0;
}
