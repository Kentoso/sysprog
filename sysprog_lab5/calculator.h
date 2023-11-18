#include <stdbool.h>

extern int yylineno;

typedef struct {
    double ans;
    bool is_reading_from_file;
    int current_line;
} calculator_data;

extern calculator_data calc_data;

struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

typedef struct ast ast;

typedef struct {
    int nodetype;
    double number;
} numval;

ast *new_ast(int nodetype, ast *l, ast *r);
ast *new_num(double d);

double eval(ast *);

void free_tree(ast *);