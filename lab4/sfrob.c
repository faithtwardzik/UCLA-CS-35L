#include <stdlib.h>
#include <stdio.h>

char unfrob(const char frobbed) {
    return frobbed ^ 42;
}

int frobcmp(char const* a, char const* b) {

    char const* first = a;
    char const* second = b;

    int i = 0;
    while (first[i] != ' ' && second[i] != ' ') {
        if (unfrob(first[i]) != unfrob(second[i]))
            return unfrob(first[i]) - unfrob(second[i]);
        else
            i++;
    }

    // if one array is longer than or equal to the other
    if (first[i] == ' ' && second[i] != ' ')
        return -1;
    else if (first[i] != ' ' && second[i] == ' ')
        return 1;
    else
        return 0;
}

// allows frobcmp to be used in qsort
int WrapperFrobcmp(const void* first, const void *second) {
    return frobcmp(*((const char**) first), *((const char**) second));
}

void PrintList(char **list, int items) {
    int i = 0;
    int j = 0;
    for (; i < items; i++, j=0) {
        for (; list[i][j] != ' '; j++) {
            printf("%c", list[i][j]);
        }
        printf(" ");
    }
}

void FreeList(char **list, int items) {
    int i = 0;
    for (; i < items; i++) {
        free(list[i]);
    }
    free(list);
}

int main() {
   
    // an array of arrays of chars to hold each word
    char **words = (char **)malloc(sizeof(char *)); 
    if (!words) {
      fprintf(stderr, "Error allocating memory");
      exit(1);
    }

    char *curr_word = (char *)malloc(sizeof(char));

    if (!curr_word) {
      fprintf(stderr, "Error allocating memory");
      exit(1);
    }

    char curr_char;
    int words_size = 0;
    int curr_word_size = 0;
    char last_char = '\0';
    int character = 0;

    while (!feof(stdin)) {

        if (ferror(stdin)) {
            fprintf(stderr, "Error reading standard input");
            exit(1);
        }

        character = getchar();    

        if (character == EOF)
            break;

        curr_char = (char)character;

	// add curr_char to the curr_word
        if (!(curr_word_size == 0 && curr_char == ' ')) {
            curr_word = (char *)realloc(curr_word, (curr_word_size + 1) * sizeof(char));
            curr_word[curr_word_size] = curr_char;
            curr_word_size++;

	    // if space, end of curr_word, add word to words array
            if (curr_char == ' ') {
                words = (char **)realloc(words, (words_size + 1) * sizeof(char*));
                words[words_size] = curr_word;
                words_size++;
                curr_word_size = 0;
                curr_word = (char *)malloc(sizeof(char));

		if (!curr_word) {
		  fprintf(stderr, "Error allocating memory");
		  exit(1);
		}
            }

            last_char = curr_char;
        }
    }

    if (last_char == ' ' || words_size == 0)
        free(curr_word);


    // append a space to last word if it doesn't have one
    if (last_char != ' ' && words_size > 0) {
        curr_word = (char *)realloc(curr_word, (curr_word_size + 1) * sizeof(char));
        curr_word[curr_word_size] = ' ';
        curr_word_size++;

        // add last word to word list
        words = (char **)realloc(words, (words_size + 1) * sizeof(char*));
        words[words_size] = curr_word;
        words_size++;
    }

    qsort(words, words_size, sizeof(char*), WrapperFrobcmp);
    PrintList(words, words_size);
    FreeList(words, words_size);
}
