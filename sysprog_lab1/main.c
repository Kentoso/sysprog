#include <wctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include "array.h"
#include <locale.h>

#define MAX_WORD_LENGTH 30

typedef enum {
    INIT,
    READING_WORD,
    READING_SEPARATORS
} States;

int main(void) {
    setlocale(LC_ALL, "");
    wchar_t word_buffer[MAX_WORD_LENGTH + 1];
    int current_buffer_position = 0;
    FILE *file;
    DynamicWStringArray *arr = init_dynamic_array(10);

    file = fopen("tests/ukr.txt", "r");
    if (file == NULL) {
        perror("File opening failed");
        return -1;
    }

    States current_state = INIT;
    wint_t c = '\0';
    bool is_alpha = false;
    int max_length = 0;
    bool running = true;
    while (running) {
        c = fgetwc(file);
        if (c == WEOF) {
            running = false;
            is_alpha = false;
        }
        else {
            is_alpha = iswalpha(c);
        }
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
            word_buffer[current_buffer_position] = L'\0';
            if (max_length < current_buffer_position) {
                max_length = current_buffer_position;
                clear(arr);
            }
            if (max_length == current_buffer_position) {
                append(arr, word_buffer);
            }
            current_buffer_position = 0;
            current_state = READING_SEPARATORS;
        }
        else if (current_buffer_position >= MAX_WORD_LENGTH) {
            fwprintf(stderr, L"Word too long. Shutting down...");
            return -1;
        }
    }

    FILE *output_file;

    output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Output file opening failed");
        return -1;
    }

    fputws(L"Max:\n", output_file);
    fwprintf(output_file, L"%d\n", max_length);
    for (int i = 0; i < arr->size; i++) {
        wprintf(L"%ls\n", arr->array[i]);
        fputws(arr->array[i], output_file);
        fputwc(L'\n', output_file);
    }

    free_dynamic_array(arr);
    fclose(file);
    fclose(output_file);
}
