%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "calculator.h"
    #include <ctype.h>

    extern FILE *yyin;
%}

%define parse.error verbose

%union {
    struct ast *a;
    double num;
}

%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%token <num> FLOATNUM
%token <num> ANS
%token NEWLINE

%type <a> expression

/* %define api.value.type { double } */

%%

calclist
    : /* nothing */
    | calclist expression NEWLINE { 
            calc_data.ans = eval($2);
            if (calc_data.is_reading_from_file) {
                printf("%d) %f\n", calc_data.current_line, calc_data.ans); 
            }
            else {
                printf("= %f\n", calc_data.ans); 
            }
            free_tree($2); 
            calc_data.current_line++;
        }
    | calclist NEWLINE
    ;
    
expression
    : expression '+' expression { $$ = new_ast('+', $1, $3); }
    | expression '-' expression { $$ = new_ast('-', $1, $3); }
    | expression '*' expression { $$ = new_ast('*', $1, $3); }
    | expression '/' expression { $$ = new_ast('/', $1, $3); }
    | '|' expression '|'        { $$ = new_ast('|', $2, NULL); }
    | '(' expression ')'        { $$ = $2; }
    | '-' expression %prec UMINUS { $$ = new_ast('M', $2, NULL); }
    | FLOATNUM                  { $$ = new_num($1); }
    | ANS                       { $$ = new_num($1); }
    ;
%%

int append_newline_if_needed(FILE *file) {
    if (fseek(file, 0, SEEK_END) != 0) {
        printf("Error seeking to end of file.\n");
        return -1;
    }

    long file_size = ftell(file);
    if (file_size == 0) {
        printf("File is empty, no newline needed.\n");
        return 0;
    }

    int is_last_line_whitespace_only = 1;
    for (long i = file_size - 1; i >= 0; --i) {
        if (fseek(file, i, SEEK_SET) != 0) {
            printf("Error seeking in file.\n");
            return -1;
        }

        int c = fgetc(file);
        if (c == '\n') {
            printf("Newline found, no additional newline needed.\n");
            break;
        } else if (!isspace(c)) {
            printf("Non-whitespace character found, appending newline.\n");
            is_last_line_whitespace_only = 0;
            break;
        }
    }

    if (is_last_line_whitespace_only) {
        if (fputc('\n', file) == EOF) {
            printf("Error writing newline to file.\n");
            return -1;
        }
        fflush(file);
        printf("Newline appended to file.\n");
    }

    rewind(file);
    return 0;
}

int main(int argc, char **argv)
{
    calc_data.is_reading_from_file = argc > 1;

    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }

    /* append_newline_if_needed(yyin); */

    do {
        yyparse();
    } while (!feof(yyin));
}

int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
}
