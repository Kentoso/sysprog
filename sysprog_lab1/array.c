#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

DynamicWStringArray *init_dynamic_array(int initial_capacity) {
    DynamicWStringArray *arr = (DynamicWStringArray *)malloc(sizeof(DynamicWStringArray));
    if (arr == NULL) {
        perror("Failed to allocate memory for the dynamic array");
        exit(EXIT_FAILURE);
    }
    arr->array = (wchar_t **)malloc(initial_capacity * sizeof(wchar_t *));
    if (arr->array == NULL) {
        perror("Failed to allocate memory for the array elements");
        free(arr);
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void append(DynamicWStringArray *arr, const wchar_t *element) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->array = (wchar_t **)realloc(arr->array, arr->capacity * sizeof(wchar_t *));
        if (arr->array == NULL) {
            perror("Failed to reallocate memory for the array elements");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < arr->size; i++) {
        if (wcscmp(element, arr->array[i]) == 0) {
            return;
        }
    }
    arr->array[arr->size] = wcsdup(element);
    arr->size++;
}

void clear(DynamicWStringArray *arr) {
    for (int i = 0; i < arr->size; i++) {
        free(arr->array[i]);
    }
    arr->size = 0;
}

void free_dynamic_array(DynamicWStringArray *arr) {
    clear(arr);
    free(arr->array);
    free(arr);
}
