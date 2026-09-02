#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Validate the user's command line arguments
    if (argc != 2) {
        fprintf(stderr, "Invalid number of arguments.\n");
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Make short name for the input
    char* input = argv[1];
    // argv[0] is the name of the program
    // char** argv means argv is a pointer to an array, each element of the array is a pointer to a string (char*).

    // Make an array of counters for our bucket sort, init to 0.
    int counters[256] = {0};

    // All strings are array of characters, so there are 2 ways to do this.
    // for(int i=0; argv[1][i] != '\0'; i++) {
    // or use strlen() but a bit less efficient since it has to traverse the string twice.

    // Count occurences of each character in the input
    // Null eliminator \0 is counters[0] but we never increment it
    for (char* p=input; *p != '\0'; p++) {
        counters[(unsigned char)*p]++;
    }

    // Print each character the number of times it appeared in the input
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < counters[i]; j++) {
            // Print the character corresponding to the ASCII value i
            printf("%c", i);
            // or use putChar(i)
        }
    }

    printf("\n");

    // no malloc so all good

    return 0;
}