/* 
Assignment 5: System Call Programming and Debugging
Grade Received: 97/100
If you found this file helpful, please consider reaching out to me:
    Website: faithtwardzik.com
    Insta: @faithtwardzik
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

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

int IgCaseFrobCmp(char const* a, char const* b) {

    char const* first = a;
    char const* second = b;

    int i = 0;
    while (first[i] != ' ' && second[i] != ' ') {
        if (toupper(unfrob((unsigned char)first[i])) != toupper(unfrob((unsigned char)second[i])))
            return toupper(unfrob((unsigned char)first[i])) - toupper(unfrob((unsigned char)second[i]));
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

// allow frobcmp and IgCaseFrobCmp to be used in qsort
int WrapperFrobcmp(const void* first, const void *second) {
    return frobcmp(*((const char**) first), *((const char**) second));
}

int IgCaseWrapperFrobcmp(const void* first, const void* second) {
    return IgCaseFrobCmp(*((const char**) first), *((const char**) second));
}

void PrintList(char **list, int items) {
    int i = 0;
    int j = 0;
    char space = ' ';
    for (; i < items; i++, j=0) {
        for (; list[i][j] != ' '; j++) {
            write(STDOUT_FILENO, &list[i][j], sizeof(char));
        }
        write(STDOUT_FILENO, &space, sizeof(char));
    }
}

void FreeList(char **list, int items) {
    int i = 0;
    for (; i < items; i++) {
        free(list[i]);
    }
    free(list);
}

// read all else (eg stdin) to a buffer
char* ReadStdin(int* end_size) {

    char* buffer = malloc(sizeof(char));
    char curr_char = '\0';
    int buf_size = 1;
    int i = 0;

    // read individual chars into buffer
    int status = read(STDIN_FILENO, &curr_char, sizeof(curr_char));
    while (status > 0) {
        buffer[i] = curr_char;
        i++;
        status = read(STDIN_FILENO, &curr_char, sizeof(curr_char));
        if (status > 0) {
            buf_size += 1;
            buffer = realloc(buffer, buf_size);
        }
    }

    *end_size = buf_size;
    return buffer;

}

// read in a regular file to a buffer
char* ReadFile(int size, int* end_size) {

    char* buffer = malloc(size);
    char curr_char = '\0';
    int i = 0;

    // read individual chars into buffer
    int status = read(STDIN_FILENO, &curr_char, sizeof(curr_char));
    while (status > 0) {
        buffer[i] = curr_char;
        i++;
        status = read(STDIN_FILENO, &curr_char, sizeof(curr_char));

        // if there is more to read yet we have reached original size,
        // file must be growing, realloc additional space
        if (status > 0 && i == size) {
            size += 1;
            buffer = realloc(buffer, size); 
        }
    }

    *end_size = size;
    return buffer;

}

int main(int argc, char* argv[]) {

    char* read_buf;
    int* buf_size_ptr;
    int buf_size = 0;
    buf_size_ptr = &buf_size;
    char foption = 'n';

    if (argc > 2) {
        char err_msg[] = "Invalid number of arguments\n";
        write(2, err_msg, sizeof(err_msg)-1);
        exit(1);
    }
    else if (argc == 2 && (strlen(argv[1]) != 2 || argv[1][0] != '-' || argv[1][1] != 'f')) {
        char err_msg[] = "Invalid argument\n";
        write(2, err_msg, sizeof(err_msg)-1);
        exit(1);
    }
    else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'f')
        foption = 'y';

    // Read all input from stdin / file to a buffer
    int size;
    size = 0;
    struct stat file_input;
    fstat(0, &file_input);
    if (S_ISREG(file_input.st_mode)) {
        size = file_input.st_size;
        read_buf = ReadFile(size, buf_size_ptr);
    }
    else {
        read_buf = ReadStdin(buf_size_ptr);
    }

    // an array of arrays of chars to hold each word
    char **words = (char **)malloc(sizeof(char *)); 
    if (!words) {
        char err_msg[] = "Error allocating memory\n";
        write(2, err_msg, sizeof(err_msg)-1);
        exit(1);
    }

    char *curr_word = (char *)malloc(sizeof(char));

    if (!curr_word) {
        char err_msg[] = "Error allocating memory\n";
        write(2, err_msg, sizeof(err_msg)-1);
        exit(1);
    }

    char curr_char;
    int words_size = 0;
    int curr_word_size = 0;
    char last_char = '\0';
    int character = 0;

    int j = 0;
    for (; j < buf_size; j++) {
        curr_char = read_buf[j];

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
                    char err_msg[] = "Error allocating memory\n";
                    write(2, err_msg, sizeof(err_msg)-1);
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

    if (foption == 'n')
        qsort(words, words_size, sizeof(char*), WrapperFrobcmp);
    else 
        qsort(words, words_size, sizeof(char*), IgCaseWrapperFrobcmp);

    PrintList(words, words_size);
    FreeList(words, words_size);
    free(read_buf);

}
