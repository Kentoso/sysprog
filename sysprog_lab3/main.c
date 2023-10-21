#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_type.h"
#include "token_table.h"

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

const char* TokenTypeNames[] = {
    "FLOAT", "EXPFLOAT", "DECINTEGER", "HEXINTEGER", "STRING", "COMMENT",
    "RESERVED", "OPERATOR", "DELIMITER", "IDENTIFIER", "UNKNOWN"
};

const char *patterns[] = {
    "\\B\\.[0-9]+\\b|\\b[0-9]+\\.\\B|\\b([1-9][0-9]*|0)\\.[0-9]+\\b",
    "(\\B\\.[0-9]+|\\b[0-9]+\\.|\\b([1-9][0-9]*|0)\\.[0-9]+|\\b([1-9][0-9]*|0))(e|E)(\\+|-)?([1-9][0-9]*|0)",
    "\\b([1-9][0-9]*|0)\\b",
    "\\b0[xX][0-9a-fA-F]+\\b",
    "(r|R|u|U|f|F|fr|Fr|fR|FR|RF|rF|Rf)?('''([^\\\\]|\\\\.)*'''|\"\"\"([^\\\\]|\\\\.)*?\"\"\"|'([^'\\\\\\n]|\\.)*'|\"([^\"\\\\\\n]|\\.)*\")",
    "#.*?$",
    "\\b(False|await|else|import|pass|None|break|except|in|raise|True|class|finally|is|return|and|continue|for|lambda|try|as|def|from|nonlocal|while|assert|del|global|not|with|async|elif|if|or|yield)\\b",
    "\\*\\*|//|<<|>>|<=|>=|==|!=|\\+|-|\\*|/|%|@|&|\\||\\^|~|:=|<|>",
    "\\*\\*=|//=|<<=|>>=|->|\\+=|-=|\\*=|/=|%=|@=|&=|\\|=|\\^=|\\(|\\)|\\[|\\]|\\{|\\}|,|:|\\.|;|@|=",
    "\\b[A-Za-z_][0-9A-Za-z_]*\\b",
    "[^\\s]+"
};

const TokenType priority[] = {
    COMMENT, STRING, RESERVED, OPERATOR, DELIMITER, EXPFLOAT, FLOAT, HEXINTEGER, DECINTEGER, IDENT, UNKNOWN
};

#define NUM_PATTERNS (sizeof(patterns)/sizeof(patterns[0]))

int get_position_info(char *content, char *start_pos, char *cursor) {
    int line_number = 1;
    
    char *temp_pos = start_pos;
    while (temp_pos < cursor) {
        if (*temp_pos == '\n') {
            line_number++;
        }
        temp_pos++;
    }

    return line_number;
}

TokenTable lex_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(fsize + 1);
    fread(content, sizeof(char), fsize, file);
    content[fsize] = '\0';
    fclose(file);

    PCRE2_SPTR cursor = (PCRE2_SPTR)content;
    
    TokenTable table;
    initialize_table(&table);

    while (*cursor) {
        int nearest_match_pos = -1;
        int nearest_match_len = -1;
        int nearest_match_type = -1;

        for (int i = 0; i < NUM_PATTERNS; i++) {
            pcre2_code *re;
            int pattern_num = priority[i];
            PCRE2_SPTR pattern = (PCRE2_SPTR)patterns[pattern_num];
            pcre2_match_data *match_data;
            int errornumber;
            PCRE2_SIZE erroroffset;

            re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, PCRE2_MULTILINE, &errornumber, &erroroffset, NULL);

            if (!re) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
                fprintf(stderr, "PCRE2 compilation failed for pattern \"%s\", offset: %d: %s\n", patterns[pattern_num], (int)erroroffset, buffer);
                continue;
            }

            match_data = pcre2_match_data_create_from_pattern(re, NULL);

            if (pcre2_match(re, cursor, strlen((char *)cursor), 0, 0, match_data, NULL) > 0) {
                PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
                int match_start = (int)ovector[0];
                int match_end = (int)ovector[1];
                int match_length = match_end - match_start;

                if (nearest_match_pos == -1 || match_start < nearest_match_pos) {
                    nearest_match_pos = match_start;
                    nearest_match_len = match_length;
                    nearest_match_type = pattern_num;
                }
            }

            pcre2_match_data_free(match_data);
            pcre2_code_free(re);
        }

        if (nearest_match_pos != -1) {
            char lexeme[1024];
            strncpy(lexeme, (char *)(cursor + nearest_match_pos), nearest_match_len);
            lexeme[nearest_match_len] = '\0';
            add_to_table(&table, lexeme, nearest_match_type);
            printf(
            "%sLexeme: \"%s\", Type: %s%s\n",
                (nearest_match_type == UNKNOWN) ? "\033[0;31m" : "",
                lexeme, 
                TokenTypeNames[nearest_match_type],
                (nearest_match_type == UNKNOWN) ? "\033[0m" : ""
            );
            if (nearest_match_type == UNKNOWN) {
                int pos = get_position_info(content, content, (char*)cursor);
                printf("%sUNKNOWN token found at line %d%s\n", "\033[0;31m", pos, "\033[0m");
            }

            cursor += (nearest_match_pos + nearest_match_len);
        } else {
            cursor += 1;
        }
    }

    free(content);
    return table;
}

int main() {
    const char *filename = "tests/test2.py";
    TokenTable table = lex_file(filename);
    for (int type = FLOAT; type <= UNKNOWN; type++) {
        print_table(&table, (TokenType)type, TokenTypeNames);
    }
    free_table(&table);
    return 0;
}