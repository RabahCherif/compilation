#include <stdio.h>

#define MAX_ST 10
#define MAX_SIG 5

int totalStates, totalSymbols;
char alphabet[MAX_SIG];

int delta[MAX_ST][MAX_SIG][MAX_ST];

int epsilonMoves[MAX_ST][MAX_ST];

int eClosure[MAX_ST][MAX_ST];

int startState;
int isFinal[MAX_ST];

void loadAutomaton() {
    int i, j, k, s, f, nbFinals;

    printf("Number of states: ");
    scanf("%d", &totalStates);
    printf("Number of symbols: ");
    scanf("%d", &totalSymbols);

    printf("Enter the symbols: ");
    for (i = 0; i < totalSymbols; i++)
        scanf(" %c", &alphabet[i]);

    for (i = 0; i < totalStates; i++) {
        isFinal[i] = 0;
        for (j = 0; j < totalSymbols; j++)
            for (k = 0; k < totalStates; k++)
                delta[i][j][k] = 0;
        for (j = 0; j < totalStates; j++)
            epsilonMoves[i][j] = 0;
    }

    printf("Initial state: ");
    scanf("%d", &startState);

    printf("Number of final states: ");
    scanf("%d", &nbFinals);
    printf("Final states: ");
    for (i = 0; i < nbFinals; i++) {
        scanf("%d", &f);
        isFinal[f] = 1;
    }

    printf("Transitions (from symbol to), -1 to stop \n");
    while (1) {
        int from, to;
        char sym;
        scanf("%d", &from);
        if (from == -1) break;
        scanf(" %c %d", &sym, &to);

        for (i = 0; i < totalSymbols; i++)
            if (alphabet[i] == sym)
                delta[from][i][to] = 1;
    }

    printf("Epsilon transitions (from to), -1 to stop \n");
    while (1) {
        int from, to;
        scanf("%d", &from);
        if (from == -1) break;
        scanf("%d", &to);
        epsilonMoves[from][to] = 1;
    }
}

void findEpsilonClosures() {
    int i, j, k, updated;

    for (i = 0; i < totalStates; i++) {
        for (j = 0; j < totalStates; j++)
            eClosure[i][j] = 0;
        eClosure[i][i] = 1;
    }

    do {
        updated = 0;
        for (i = 0; i < totalStates; i++)
            for (j = 0; j < totalStates; j++)
                if (eClosure[i][j])
                    for (k = 0; k < totalStates; k++)
                        if (epsilonMoves[j][k] && !eClosure[i][k]) {
                            eClosure[i][k] = 1;
                            updated = 1;
                        }
    } while (updated);
}

void rebuildTransitions() {
    int i, j, k, s, d;
    int oldDelta[MAX_ST][MAX_SIG][MAX_ST];


    for (i = 0; i < totalStates; i++)
        for (j = 0; j < totalSymbols; j++)
            for (k = 0; k < totalStates; k++)
                oldDelta[i][j][k] = delta[i][j][k];

    for (i = 0; i < totalStates; i++)
        for (j = 0; j < totalSymbols; j++)
            for (k = 0; k < totalStates; k++)
                delta[i][j][k] = 0;

    for (i = 0; i < totalStates; i++)
        for (s = 0; s < totalStates; s++)
            if (eClosure[i][s])
                for (j = 0; j < totalSymbols; j++)
                    for (d = 0; d < totalStates; d++)
                        if (oldDelta[s][j][d])
                            for (k = 0; k < totalStates; k++)
                                if (eClosure[d][k])
                                    delta[i][j][k] = 1;
}

void updateFinals() {
    int i, j;
    for (i = 0; i < totalStates; i++)
        for (j = 0; j < totalStates; j++)
            if (eClosure[i][j] && isFinal[j])
                isFinal[i] = 1;
}

void showAutomaton() {
    int i, j, k;

    printf("\n--- Resulting NFA without epsilon transitions ---\n");
    printf("Initial state: %d\n", startState);

    printf("Final states: ");
    for (i = 0; i < totalStates; i++)
        if (isFinal[i]) printf("%d ", i);
    printf("\n");

    printf("Transitions:\n");
    for (i = 0; i < totalStates; i++)
        for (j = 0; j < totalSymbols; j++)
            for (k = 0; k < totalStates; k++)
                if (delta[i][j][k])
                    printf("%d --%c--> %d\n", i, alphabet[j], k);
}

int main() {
    loadAutomaton();
    findEpsilonClosures();
    rebuildTransitions();
    updateFinals();
    showAutomaton();
    return 0;
}
