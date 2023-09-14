#include <stdio.h>


typedef struct {
    const char **array;    // Pointer to the underlying dynamic array
    int size;   // Current number of elements in the array
    int capacity; // Capacity of the array (maximum elements it can hold)
} DynamicCStringArray;

DynamicCStringArray *init_dynamic_array(int initial_capacity);

void append(DynamicCStringArray *arr, const char *element);

void free_dynamic_array(DynamicCStringArray *arr);

void clear(DynamicCStringArray *arr);