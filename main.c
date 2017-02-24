#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Global variables to deal with states */
int size, *queens, *curr, *clon;

/* Print the chess board */
void print(int *state, int a) {
    int i, j;

    printf("\n\nNumber of attacks: %d\n\n", a);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) printf("+--");
        printf("+\n|");
        for (j = 0; j < size; j++) {
            if (state[j] == i) printf(" o|");
            else printf("  |");
        }
        printf("\n");
    }
    for (j = 0; j < size; j++) printf("+--");
    printf("+\n\n");
}

/* Randomize the queens in the Global state 'queens' */
void randomize() {
    int i;
    for (i = 0; i < size; i++)
        queens[i] = rand() % size;
}

/* Returns a min and max index value based on the board size. */
int min(int v) {
    if(v < 0) return 0;
    return v;
}
int max(int v){
    if(v > size-1) return size-1;
    return v;
}

/* Calculates the number of attacks given a board */
int attacks(int *q){
    int i, j, atk = 0, cmi, lmi, cma, lma;
    for (i = 0; i < size; i++) {

        for (j = 0; j < size; j++)
            if (q[i] == q[j] && i != j) atk++;

        cmi = cma = i;
        lmi = lma = q[i];
        while (min(cmi) != 0 || min(lmi) != 0 || max(cma) != size-1 || max(lma) != size-1) {
            cmi--; lmi--; cma++; lma++;
            if (lmi > -1 && cmi > -1) if (q[cmi] == lmi) atk++;
            if (lma < size && cmi > -1) if (q[cmi] == lma) atk++;
            if (lmi > -1 && cma < size) if (q[cma] == lmi) atk++;
            if (lma < size && cma < size) if (q[cma] == lma) atk++;
        }

    }

    return atk;
}

/* Copy the data of a state to another */
void copystate(int *from, int *to){
    int i;
    for (i = 0; i < size; i++)
        to[i] = from[i];
}

/* Expands a given state and chooses the best of them to expand again.
   If there is no best state, return the current state to initialize
   the board again.
   This is the Hill-Climbing algorithm. */
int *expand(int *state){
    int i, j, found = 0, atkclon;

    copystate(state, curr);
    int atkcurr = attacks(curr);

    // Get all possible states from the current state
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            copystate(state, clon);
            clon[i] = j;
            atkclon = attacks(clon);
            if (atkclon == 0) return clon;
            else if (atkclon < atkcurr) {
                copystate(clon, curr);
                atkcurr = atkclon;
                found = 1;
            }
        }
    }

    if (found == 1) return expand(curr);
    else return curr;
}


int main() {
    int atk, re = 0;
    clock_t t;
    srand(time(NULL));

    printf("Number of queens: ");
    scanf("%d", &size);

    // Allocate the vectors that represents the states
    queens = (int*) malloc (sizeof(int) * size);
    curr   = (int*) malloc (sizeof(int) * size);
    clon   = (int*) malloc (sizeof(int) * size);

    t = clock();
    // Reinitialize until the number of attacks on the board is zero
    do {
        randomize();
        queens = expand(queens);
        atk = attacks(queens);
        re++;
    } while (atk != 0);
    t = clock() - t;

    printf("\n\nThe algorithm restarted %d time(s)!\n\nAnd took %f seconds to solve the problem.\n", re, ((float)t)/CLOCKS_PER_SEC);
    print(queens, atk);


    /*FILE *f;
    int i;
    f = fopen("dados.txt", "a");
    fprintf(f, "%d-%d-%f-[", size, re, ((float)t)/CLOCKS_PER_SEC);
    for (i = 0; i < size; i++) fprintf(f, "%d,", queens[i]);
    fprintf(f, "]\n");
    fclose(f);*/


    return 0;
}
