#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calculator.h"

calculator_data calc_data;

ast *new_ast(int nodetype, ast *l, ast *r) {
    ast *a = malloc(sizeof(ast));
    if (!a) {
        fprintf(stderr, "%d: error: malloc failed", calc_data.current_line);
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

ast *new_num(double d) {
    numval *a = malloc(sizeof(numval));
    if (!a) {
        fprintf(stderr, "%d: error: malloc failed", calc_data.current_line);
        exit(0);
    }
    a->nodetype = 'N';
    a->number = d;
    return (ast *)a;
}

double eval(ast *a) {
    double v;
    switch (a->nodetype) {
        case 'N':
            v = ((numval *)a)->number;
            break;
        case '+':
            v = eval(a->l) + eval(a->r);
            break;
        case '-':
            v = eval(a->l) - eval(a->r);
            break;
        case '*':
            v = eval(a->l) * eval(a->r);
            break;
        case '/':
            v = eval(a->l) / eval(a->r);
            break;
        case '|':
            v = eval(a->l);
            if (v < 0) v = -v;
            break;
        case 'M':
            v = -eval(a->l);
            break;
        default:
            printf("internal error: bad node %c\n", a->nodetype);
    }
    return v;
}

void free_tree(ast *a) {
    switch (a->nodetype) {
        case '+':
        case '-':
        case '*':
        case '/':
            free_tree(a->r);
        case '|':
        case 'M':
            free_tree(a->l);
        case 'N':
            free(a);
            break;
        default:
            printf("internal error: free bad node %c\n", a->nodetype);
    }
}
