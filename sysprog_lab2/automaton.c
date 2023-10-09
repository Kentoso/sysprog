// #include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "automaton.h"

bool automaton_init(FILE *file, Automaton *automaton) {
    if (file == NULL || automaton == NULL) {
        return false;
    }

    automaton->numAlphabet = 0;
    automaton->numStates = 0;
    automaton->initialState = -1;
    automaton->numFinalStates = 0;
    automaton->numTransitions = 0;

    printf("• Reading automaton from file...\n");

    fscanf(file, "%d", &automaton->numAlphabet);
    for (int i = 0; i < automaton->numAlphabet; i++) {
        fscanf(file, " %c", &automaton->alphabet[i]);
    }

    fscanf(file, "%d", &automaton->numStates);
    for (int i = 0; i < automaton->numStates; i++) {
        fscanf(file, " %d", &automaton->states[i]);
    }

    fscanf(file, "%d", &automaton->initialState);

    fscanf(file, "%d", &automaton->numFinalStates);
    for (int i = 0; i < automaton->numFinalStates; i++) {
        fscanf(file, " %d", &automaton->finalStates[i]);
    }

    fscanf(file, "%d", &automaton->numTransitions);

    for (int i = 0; i < automaton->numTransitions; i++) {
        fscanf(file, "%d %c %d", &automaton->transitions[i].fromState,
               &automaton->transitions[i].inputSymbol,
               &automaton->transitions[i].toState);
    }

    print_automaton_info(automaton);

    printf("• Automaton initialization completed\n");

    return true;
}

void print_automaton_info(const Automaton *automaton) {
    printf("Alphabet: ");
    for (int i = 0; i < automaton->numAlphabet; i++) {
        printf("%c ", automaton->alphabet[i]);
    }
    printf("\n");

    printf("States: ");
    for (int i = 0; i < automaton->numStates; i++) {
        printf("%d ", automaton->states[i]);
    }
    printf("\n");

    printf("Initial State: %d\n", automaton->initialState);

    printf("Final States: ");
    for (int i = 0; i < automaton->numFinalStates; i++) {
        printf("%d ", automaton->finalStates[i]);
    }
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < automaton->numTransitions; i++) {
        printf("%d -->|%c| %d\n", automaton->transitions[i].fromState, automaton->transitions[i].inputSymbol, automaton->transitions[i].toState);
    }
}

bool evaluate_string(const Automaton *automaton, const char *input) {
    int currentState = automaton->initialState;

    for (int i = 0; input[i] != '\0'; i++) {
        char currentSymbol = input[i];
        bool validTransition = false;

        for (int j = 0; j < automaton->numTransitions; j++) {
            if (automaton->transitions[j].fromState == currentState &&
                automaton->transitions[j].inputSymbol == currentSymbol) {
                currentState = automaton->transitions[j].toState;
                validTransition = true;
                break;
            }
        }

        if (!validTransition) {
            return false;
        }
    }

    for (int i = 0; i < automaton->numFinalStates; i++) {
        if (currentState == automaton->finalStates[i]) {
            return true;
        }
    }

    return false;
}

bool evaluate_file(FILE *file, const Automaton *automaton) {
   char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (!evaluate_string(automaton, buffer)) {
            return false;
        }
    }

    return true;
}

bool find_shortest_recognizable_word(const Automaton *automaton, char *result, int maxLength) {
    if (maxLength <= 0) {
        return false;
    }

    result[0] = '\0';
    bool wasResultTouched = false;

    bool visited[MAX_STATES];
    for (int i = 0; i < automaton->numStates; i++) {
        visited[i] = false;
    }

    char paths[MAX_STATES][MAX_WORD_LENGTH];
    for (int i = 0; i < automaton->numStates; i++) {
        paths[i][0] = '\0';
    }

    int queue[MAX_STATES];
    int front = 0, rear = 0;

    queue[rear] = automaton->initialState;
    rear++;
    visited[automaton->initialState] = true;

    while (front < rear) {
        int currentState = queue[front];

        for (int i = 0; i < automaton->numTransitions; i++) {
            if (automaton->transitions[i].fromState == currentState && !visited[automaton->transitions[i].toState]) {
                if (strlen(paths[currentState]) + 1 < maxLength) {
                    strcpy(paths[automaton->transitions[i].toState], paths[currentState]);
                    strcat(paths[automaton->transitions[i].toState], &(automaton->transitions[i].inputSymbol));
                }

                visited[automaton->transitions[i].toState] = true;
                queue[rear] = automaton->transitions[i].toState;
                rear++;
            }
        }

        front++;
    }
    int min_word_length = INT_MAX;
    for (int i = 0; i < automaton->numFinalStates; i++) {
        int length = strlen(paths[automaton->finalStates[i]]);
        if (length > 0) {
            min_word_length = length < min_word_length ? length : min_word_length;
        }
    }

    for (int i = 0; i < automaton->numFinalStates; i++) {
        int length = strlen(paths[automaton->finalStates[i]]);
        if (length == min_word_length) {
            wasResultTouched = true;
            strcat(result, paths[automaton->finalStates[i]]);
            strcat(result, ", ");
        }
    }

    if (strlen(result) > 0) {
        result[strlen(result) - 2] = '\0';
    }

    return wasResultTouched;
}

bool intersect_automata(const Automaton *automaton1, const Automaton *automaton2, Automaton *result) {
    result->numAlphabet = automaton1->numAlphabet;
    for (int i = 0; i < result->numAlphabet; i++) {
        result->alphabet[i] = automaton1->alphabet[i];
    }
    result->numStates = 0;
    result->numFinalStates = 0;
    result->numTransitions = 0;

    int stateMap[MAX_STATES][MAX_STATES];
    for (int i = 0; i < automaton1->numStates; i++) {
        for (int j = 0; j < automaton2->numStates; j++) {
            stateMap[i][j] = result->numStates;
            result->states[result->numStates] = result->numStates;
            result->numStates++;
        }
    }

    result->initialState = stateMap[automaton1->initialState][automaton2->initialState];

    for (int i = 0; i < automaton1->numTransitions; i++) {
        for (int j = 0; j < automaton2->numTransitions; j++) {
            if (automaton1->transitions[i].inputSymbol != automaton2->transitions[j].inputSymbol) {
                continue;
            }

            int newState1 = stateMap[automaton1->transitions[i].fromState][automaton2->transitions[j].fromState];
            int newState2 = stateMap[automaton1->transitions[i].toState][automaton2->transitions[j].toState];

            result->transitions[result->numTransitions].fromState = newState1;
            result->transitions[result->numTransitions].inputSymbol = automaton1->transitions[i].inputSymbol;
            result->transitions[result->numTransitions].toState = newState2;
            result->numTransitions++;
        }
    }

    for (int i = 0; i < automaton1->numFinalStates; i++) {
        for (int j = 0; j < automaton2->numFinalStates; j++) {
            result->finalStates[result->numFinalStates] = stateMap[automaton1->finalStates[i]][automaton2->finalStates[j]];
            result->numFinalStates++;
        }
    }

    return true;
}

bool write_automaton_to_file(FILE *file, const Automaton *automaton) {
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d\n", automaton->numAlphabet);
    for (int i = 0; i < automaton->numAlphabet; i++) {
        fprintf(file, "%c ", automaton->alphabet[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "%d\n", automaton->numStates);
    for (int i = 0; i < automaton->numStates; i++) {
        fprintf(file, "%d ", automaton->states[i]);
    }
    fprintf(file, "\n");

    fprintf(file, "%d\n", automaton->initialState);

    fprintf(file, "%d\n", automaton->numFinalStates);
    for (int i = 0; i < automaton->numFinalStates; i++) {
        fprintf(file, "%d ", automaton->finalStates[i]);
    }
    fprintf(file, "\n");

    for (int i = 0; i < automaton->numTransitions; i++) {
        fprintf(file, "%d %c %d\n", automaton->transitions[i].fromState,
                automaton->transitions[i].inputSymbol,
                automaton->transitions[i].toState);
    }

    return true;
}