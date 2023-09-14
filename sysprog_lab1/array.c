#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"


DynamicCStringArray *init_dynamic_array(int initial_capacity) {
    DynamicCStringArray *arr = malloc(sizeof(DynamicCStringArray));
    if (arr == NULL) {
        perror("Failed to allocate memory for the dynamic array");
        exit(EXIT_FAILURE);
    }
    arr->array = malloc(initial_capacity * sizeof(const char *));
    if (arr->array == NULL) {
        perror("Failed to allocate memory for the array elements");
        free(arr);
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void append(DynamicCStringArray *arr, const char *element) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->array = realloc(arr->array, arr->capacity * sizeof(const char *));
        if (arr->array == NULL) {
            perror("Failed to reallocate memory for the array elements");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < arr->size; i++) {
        if (strcmp(element, arr->array[i]) == 0) {
            return;
        }
    }
    arr->array[arr->size] = strdup(element);
    arr->size++;
}

void clear(DynamicCStringArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        free((void*)arr->array[i]);
    }
    arr->size = 0;
}

void free_dynamic_array(DynamicCStringArray *arr) {
    clear(arr);
    free(arr->array);
    free(arr);
}