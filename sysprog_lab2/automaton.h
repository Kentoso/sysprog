#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_ALPHABET_SIZE 26
#define MAX_STATES 100
#define MAX_WORD_LENGTH 100

typedef struct {
    int fromState;
    char inputSymbol;
    int toState;
} Transition;

typedef struct {
    int numAlphabet;
    char alphabet[MAX_ALPHABET_SIZE];
    int numStates;
    int states[MAX_STATES];
    int initialState;
    int numFinalStates;
    int finalStates[MAX_STATES];
    int numTransitions;
    Transition transitions[MAX_STATES * MAX_ALPHABET_SIZE];
} Automaton;

bool automaton_init(FILE *file, Automaton *automaton);

void print_automaton_info(const Automaton *automaton);

bool evaluate_string(const Automaton *automaton, const char *input);

bool evaluate_string_log(const Automaton *automaton, const char *input);

bool evaluate_file(FILE *file, const Automaton *automaton);

bool find_shortest_recognizable_word(const Automaton *automaton, char *result, int maxLength);

bool intersect_automata(const Automaton *automaton1, const Automaton *automaton2, Automaton *result);

bool write_automaton_to_file(FILE *file, const Automaton *automaton);

#endif /* AUTOMATON_H */