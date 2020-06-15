#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  
  char curr_char;
  int int_curr_char;
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

    int_curr_char = getchar();
    while (int_curr_char != EOF) {
      curr_char = (char)int_curr_char;
      int i = 0;
      for (; i < strlen(set1); i++) {
	if (curr_char == set1[i]) {
	  putchar(set2[i]);
	  need_to_put = 'n';
	}
      }
      if (need_to_put != 'n')
      putchar(curr_char);
      
      need_to_put = 'y';
      int_curr_char = getchar();
      
    }

    return 0;
  
}
