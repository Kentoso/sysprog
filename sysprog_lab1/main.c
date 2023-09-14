#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "array.h"

#define MAX_WORD_LENGTH 30


typedef enum {
    INIT,
    READING_WORD,
    READING_SEPARATORS
} States;

int main(void) {
    char word_buffer[MAX_WORD_LENGTH + 1];
    int current_buffer_position = 0;
    FILE *file;
    DynamicCStringArray *arr = init_dynamic_array(10);
    file = fopen("tests/repeated.txt", "r");

    if (file == NULL) {
        perror("File opening failed");
        return -1;
    }
    
    States current_state = INIT;
    char c = '\0';
    bool is_alpha = false;
    int max_length = 0;
    while ((c = (char)fgetc(file)) != EOF) {
        is_alpha = isalpha(c);
        if (current_state == INIT) {
            current_state = is_alpha ? READING_WORD : READING_SEPARATORS;
        } 
        if (is_alpha && current_buffer_position < MAX_WORD_LENGTH) {
            if (current_state == READING_SEPARATORS) {
                current_state = READING_WORD;
            }
            word_buffer[current_buffer_position] = c;
            current_buffer_position += 1;
        }
        else if (!is_alpha && current_state == READING_WORD) {
            word_buffer[current_buffer_position] = '\0';
            // printf("%s\n", word_buffer);
            if (max_length < current_buffer_position) {
                max_length = current_buffer_position;
                clear(arr);
                // printf("%d\n", max_length);
            }
            if (max_length == current_buffer_position) {
                append(arr, word_buffer);
            }
            current_buffer_position = 0;
            current_state = READING_SEPARATORS;
        }
        else if (current_buffer_position >= MAX_WORD_LENGTH) {
            fprintf(stderr, "Word too long. Shutting down...");
            return -1;
        }
    }
    printf("Max:\n");
    printf("%d\n", max_length);
    for (int i = 0; i < arr->size; i++) {
        printf("%s\n", arr->array[i]);
    }
    free_dynamic_array(arr);
    fclose(file);
}