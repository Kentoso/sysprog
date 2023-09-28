#include <wchar.h>

typedef struct {
    wchar_t **array;
    int size;
    int capacity;
} DynamicWStringArray;

DynamicWStringArray *init_dynamic_array(int initial_capacity);

void append(DynamicWStringArray *arr, const wchar_t *element);

void free_dynamic_array(DynamicWStringArray *arr);

void clear(DynamicWStringArray *arr);
