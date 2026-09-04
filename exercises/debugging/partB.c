#include <stdio.h>
#include <stdlib.h>

int sum_array(int* arr, int length) {
  int sum = 0;
  for (int i = 0; i < length; i++) {
    sum += arr[i];
  }
  return sum;
}

int main() {
  // Declare two arrays, one intialized and one not
  int array1[] = {1, 2, 3};
  int array2[5];

  // Copy values from array1 to array2
  for (int i = 0; i < sizeof(array1) / sizeof(array1[0]); i++) {
    array2[i] = array1[i];
  }

  // Double check to make sure we copied our array properly.
  // We'll do this by adding them up and comparing the sums.
  int array1_sum = sum_array(array1, 3);
  int array2_sum = sum_array(array2, 3);

  // Check the result
  if (array1_sum == array2_sum) {
    printf("It works!\n");
  } else {
    printf("I've made a huge mistake.\n");
  }

  return 0;
}
