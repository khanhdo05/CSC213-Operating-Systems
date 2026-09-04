#include <stdio.h>
#include <stdlib.h>

// This struct is an entry in an array of word count information. Each instance holds a word, and
// the number of times that word occurred in a document
typedef struct word_count {
  char* word;
  int count;
} word_count_t;

// Calculate the total number of characters in a collection of words.
int total_characters(word_count_t* words, int num_words) {
  // Initialize the character count
  int count = 0;

  // Loop over the entries in the array of word counts
  for (int i = 0; i < num_words; i++) {
    // Loop over the characters in each word, up until the null terminator
    int j = 0;
    // For each character in the word, update the total number of characters
    while (words[i].word[j++] != '\0') count += words[i].count;
  }

  // Return the total number of characters
  return count;
}

int main() {
  // Create an array of word counts
  word_count_t words[10];

  // Fill in the words and their counts
  words[0].word = "hello";
  words[0].count = rand() % 50;

  words[1].word = "test";
  words[1].count = rand() % 50;

  words[2].word = "class";
  words[2].count = rand() % 50;

  words[3].word = "the";
  words[3].count = rand() % 50;

  words[4].word = "world";
  words[4].count = rand() % 50;

  words[5].word = "goodbye";
  words[5].count = rand() % 50;

  words[6].word = "operating";
  words[6].count = rand() % 50;

  words[7].word = "systems";
  words[7].count = rand() % 50;

  words[8].word = "parallel";
  words[8].count = rand() % 50;

  words[9].word = "algorithms";
  words[9].count = rand() % 50;

  // Count up all the characters in the source document
  int total = total_characters(words, 10);
  printf("Counted %d words\n", total);
}