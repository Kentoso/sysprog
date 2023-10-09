#include "automaton.h"

void test_automaton(const Automaton *automaton, const char *text) {
    printf("• Testing automaton\n");
    bool result = evaluate_string(automaton, text);
    printf("Test String: \"%s\", %s\n", text, result ? "Accepted" : "Not Accepted");
}

void automaton_demostration(const char *automaton_path) {
    FILE *file = fopen(automaton_path, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    Automaton automaton;
    bool success = automaton_init(file, &automaton);

    fclose(file);

    if (!success) {
        printf("Error initializing automaton from file\n");
        return;
    }

    const char *testString1 = "a";
    const char *testString2 = "abb";
    const char *testString3 = "ab";

    test_automaton(&automaton, testString1);
    test_automaton(&automaton, testString2);
    test_automaton(&automaton, testString3);

    char min_word[MAX_WORD_LENGTH];
    bool was_shortest_found = find_shortest_recognizable_word(&automaton, min_word, MAX_WORD_LENGTH);

    if (was_shortest_found) {
        printf("Shortest word: %s\n", min_word);
    }
    else {
        printf("Unable to find shortest word\n");
    }
}

void automata_intersect_demonstration(const char *automaton1_path, const char *automaton2_path) {
    printf("• Automata intersection demonstration\n");

    FILE *file1 = fopen(automaton1_path, "r");
    if (file1 == NULL) {
        perror("Error opening the file for automaton1");
        return;
    }

    Automaton automaton1;
    bool success1 = automaton_init(file1, &automaton1);
    fclose(file1);

    if (!success1) {
        printf("Error initializing automaton1 from file\n");
        return;
    }

    FILE *file2 = fopen(automaton2_path, "r");
    if (file2 == NULL) {
        perror("Error opening the file for automaton2");
        return;
    }

    Automaton automaton2;
    bool success2 = automaton_init(file2, &automaton2);
    fclose(file2);

    if (!success2) {
        printf("• Error initializing automaton2 from file\n");
        return;
    }

    Automaton result;
    printf("• Intersecting\n");
     printf("• Intersection automaton created successfully\n");
    print_automaton_info(&result);
    // test_automaton(&result, "");
    // test_automaton(&result, "ab");
    // test_automaton(&result, "abc");

    char min_word[MAX_WORD_LENGTH];
    bool was_shortest_found = find_shortest_recognizable_word(&result, min_word, MAX_WORD_LENGTH);

    if (was_shortest_found) {
        printf("Shortest words: %s\n", min_word);
    }
    else {
        printf("Unable to find shortest word\n");
    }

    FILE *output = fopen("./output.txt", "w");
    if (output == NULL) {
        perror("Error opening the file for output");
        return;
    }
    write_automaton_to_file(output, &result);
    fclose(output);
}

int main() {
    // automaton_demostration("./automata/(ab)plusMOD.txt");
    automaton_demostration("./automata/(ab)astMOD.txt");
    // automaton_demostration("./automata/(ab)plus.txt");
    // automaton_demostration("./automata/(ab)asterisk.txt");
    // automata_intersect_demonstration("./automata/(ab)plus.txt", "./automata/(ab)asterisk.txt");
    // automata_intersect_demonstration("./automata/(ab)plus.txt", "./automata/aasterisk.txt");
    return 0;
}
