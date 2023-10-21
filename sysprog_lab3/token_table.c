#include "token_table.h"

void initialize_table(TokenTable* table) {
    table->size = 0;
    table->capacity = 10;
    table->entries = (TokenEntry*)malloc(table->capacity * sizeof(TokenEntry));
}

void add_to_table(TokenTable* table, const char* token, TokenType type) {
    if (table->size == table->capacity) {
        table->capacity *= 2;
        table->entries = (TokenEntry*)realloc(table->entries, table->capacity * sizeof(TokenEntry));
    }

    table->entries[table->size].token = strdup(token);
    table->entries[table->size].type = type;
    table->size++;
}

void free_table(TokenTable* table) {
    for (size_t i = 0; i < table->size; i++) {
        free(table->entries[i].token);
    }
    free(table->entries);
    table->size = 0;
    table->capacity = 0;
}

void print_table(const TokenTable* table, TokenType type, const char** TokenTypeNames) {
    printf("+-----------------+---------------------------------------+\n");
    printf("| Token Type      | Lexeme                                 |\n");
    printf("+-----------------+---------------------------------------+\n");

    for (size_t i = 0; i < table->size; i++) {
        if (table->entries[i].type == type) {
            printf("| %-15s | %-37s |\n", TokenTypeNames[type], table->entries[i].token);
        }
    }

    printf("+-----------------+---------------------------------------+\n\n");
}