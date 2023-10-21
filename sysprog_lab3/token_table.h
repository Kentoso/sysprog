#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_type.h"

typedef struct {
    char* token;
    TokenType type;
} TokenEntry;

typedef struct {
    TokenEntry* entries;
    size_t size;
    size_t capacity;
} TokenTable;

void initialize_table(TokenTable* table);
void add_to_table(TokenTable* table, const char* token, TokenType type);
void free_table(TokenTable* table);
void print_table(const TokenTable* table, TokenType type, const char** TokenTypeNames);


#endif // TOKEN_TABLE_H
