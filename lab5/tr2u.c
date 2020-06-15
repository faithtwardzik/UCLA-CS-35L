/* 
Assignment 5: System Call Programming and Debugging
Grade Received: 97/100
If you found this file helpful, please consider reaching out to me:
    Website: faithtwardzik.com
    Insta: @faithtwardzik
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  
  char curr_char = '\0';
  char* set1 = argv[1];
  char* set2 = argv[2];
  char need_to_put = 'y';

  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments");
    exit(1);
  }

  if (strlen(argv[1]) != strlen(argv[2])) {
      fprintf(stderr, "Arguments not of equal length");
      exit(1);
    }

  int temp = 0;
  for (; temp < strlen(set1); temp++) {
    int j = 0;
    for (; j < strlen(set1); j++) {
      if (set1[temp] == set1[j] && temp != j) {
	fprintf(stderr, "Duplicates in Arg 1 not allowed\n");
	exit(1);
      }
    }
  }

  while (read(STDIN_FILENO, &curr_char, sizeof(curr_char)) > 0) {
    int i = 0;
    for (; i < strlen(set1); i++) {
      if (curr_char == set1[i]) {
	write(STDOUT_FILENO, &set2[i], sizeof(curr_char));
	need_to_put = 'n';
      }
    }
    if (need_to_put != 'n')
      write(STDOUT_FILENO, &curr_char, sizeof(curr_char));

    need_to_put = 'y';
  }

  return 0;

}  
