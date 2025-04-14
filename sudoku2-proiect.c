#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DIMENSIUNE 9

// Structura pentru profilul jucătorului
struct Jucator {
    char nume[50];
    int scor;
};
struct Jucator jucator;

// Declararea funcțiilor
void afiseazaTabla(int tabla[DIMENSIUNE][DIMENSIUNE]);
int validPeRand(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int numar);
int validPeColoana(int tabla[DIMENSIUNE][DIMENSIUNE], int col, int numar);
int validInSubGrila(int tabla[DIMENSIUNE][DIMENSIUNE], int startRand, int startCol, int numar);
int mutareValida(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int col, int numar);
int rezolvaSudoku(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int col);
void genereazaSudoku(int tabla[DIMENSIUNE][DIMENSIUNE]);
void copiazaTabla(int sursa[DIMENSIUNE][DIMENSIUNE], int destinatie[DIMENSIUNE][DIMENSIUNE]);
void eliminaNumere(int tabla[DIMENSIUNE][DIMENSIUNE], int indicii);
int tablaPlina(int tabla[DIMENSIUNE][DIMENSIUNE]);
int tablaCorecta(int tabla[DIMENSIUNE][DIMENSIUNE]);
void mutareUtilizator(int tabla[DIMENSIUNE][DIMENSIUNE], int original[DIMENSIUNE][DIMENSIUNE]);
void afiseazaReguli();
int alegeDificultate();
void afiseazaMeniu();
void afiseazaMeniuPauza();
int vreaSaJoaceDinNou();
void cereProfil();
void punctajPeMutare(int scorAdaugat);
void afiseazaScorFinal(int scorMaxim);


// Implamentari functii
void afiseazaTabla(int tabla[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++) {
        if (i % 3 == 0 && i != 0) printf("---------------------\n");
        for (int j = 0; j < DIMENSIUNE; j++) {
            if (j % 3 == 0 && j != 0) printf("| ");
            if (tabla[i][j] == 0)
                printf(". ");
            else
                printf("%d ", tabla[i][j]);
        }
        printf("\n");
    }
}

int validPeRand(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int numar) {
    for (int col = 0; col < DIMENSIUNE; col++)
        if (tabla[rand][col] == numar) return 0;
    return 1;
}

int validPeColoana(int tabla[DIMENSIUNE][DIMENSIUNE], int col, int numar) {
    for (int rand = 0; rand < DIMENSIUNE; rand++)
        if (tabla[rand][col] == numar) return 0;
    return 1;
}

int validInSubGrila(int tabla[DIMENSIUNE][DIMENSIUNE], int startRand, int startCol, int numar) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (tabla[i + startRand][j + startCol] == numar) return 0;
    return 1;
}

int mutareValida(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int col, int numar) {
    return validPeRand(tabla, rand, numar) &&
           validPeColoana(tabla, col, numar) &&
           validInSubGrila(tabla, rand - rand % 3, col - col % 3, numar);
}

int rezolvaSudoku(int tabla[DIMENSIUNE][DIMENSIUNE], int rand, int col) {
    if (rand == DIMENSIUNE) return 1;
    if (col == DIMENSIUNE) return rezolvaSudoku(tabla, rand + 1, 0);
    if (tabla[rand][col] != 0) return rezolvaSudoku(tabla, rand, col + 1);

    for (int num = 1; num <= 9; num++) {
        if (mutareValida(tabla, rand, col, num)) {
            tabla[rand][col] = num;
            if (rezolvaSudoku(tabla, rand, col + 1)) return 1;
            tabla[rand][col] = 0;
        }
    }
    return 0;
}

void copiazaTabla(int sursa[DIMENSIUNE][DIMENSIUNE], int destinatie[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            destinatie[i][j] = sursa[i][j];
}

void genereazaSudoku(int tabla[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            tabla[i][j] = 0;
    rezolvaSudoku(tabla, 0, 0);
}

void eliminaNumere(int tabla[DIMENSIUNE][DIMENSIUNE], int indicii) {
    while (indicii > 0) {
        int linie = rand() % DIMENSIUNE;
        int coloana = rand() % DIMENSIUNE;
        if (tabla[linie][coloana] != 0) {
            tabla[linie][coloana] = 0;
            indicii--;
        }
    }
}

int tablaPlina(int tabla[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            if (tabla[i][j] == 0) return 0;
    return 1;
}

int tablaCorecta(int tabla[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++) {
            int numar = tabla[i][j];
            if (numar != 0) {
                tabla[i][j] = 0;
                if (!mutareValida(tabla, i, j, numar)) return 0;
                tabla[i][j] = numar;
            }
        }
    return 1;
}

void punctajPeMutare(int scorAdaugat) {
    printf("+%d puncte! Scor curent: %d\n", scorAdaugat, jucator.scor);
}

void afiseazaScorFinal(int scorMaxim) {
    float procentaj = ((float)jucator.scor / scorMaxim) * 100;
    printf("\nScor final: %d / %d (%.2f%%)\n", jucator.scor, scorMaxim, procentaj);
    if (procentaj >= 90)
        printf(" Maestru Sudoku, %s!\n", jucator.nume);
    else if (procentaj >= 70)
        printf("Bine jucat, %s!\n", jucator.nume);
    else
        printf("Mai incearca, %s!\n", jucator.nume);
}

void mutareUtilizator(int tabla[DIMENSIUNE][DIMENSIUNE], int original[DIMENSIUNE][DIMENSIUNE]) {
    int rand, col, numar;
    int scorMaxim = 0;
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            if (original[i][j] == 0)
                scorMaxim += 10;

    while (!tablaPlina(tabla)) {
        printf("Introduceti rand (1-9), coloana (1-9) si numarul (1-9) sau 0 pentru meniu:\n");
        scanf("%d %d %d", &rand, &col, &numar);
        if (rand == 0 && col == 0 && numar == 0) {
            afiseazaMeniuPauza();
            break;
        }
        rand--; col--;
        if (original[rand][col] == 0 && mutareValida(tabla, rand, col, numar)) {
            tabla[rand][col] = numar;
            jucator.scor += 10;
            punctajPeMutare(10);
            afiseazaTabla(tabla);
        } else {
            printf("Mutare invalida!\n");
        }
    }

    if (tablaPlina(tabla) && tablaCorecta(tabla)) {
        printf("Felicitari! Ai completat corect Sudoku!\n");
    } else {
        printf("Sudoku incomplet sau incorect.\n");
    }

    afiseazaScorFinal(scorMaxim);
}

void afiseazaMeniuPauza() {
    int optiune;
    printf("\n--- MENIU ---\n");
    printf("1. Continuare joc\n");
    printf("2. Salvare joc\n");
    printf("3. Iesire\n");
    scanf("%d", &optiune);
    switch (optiune) {
        case 1: break;
        case 2: printf("Joc salvat! (simulat)\n"); break;
        case 3:
            if (vreaSaJoaceDinNou()) {
                printf("Jocul va incepe din nou!\n");
            } else {
                printf("La revedere!\n");
                exit(0);
            }
        default:
            printf("Optiune invalida.\n");
    }
}

int vreaSaJoaceDinNou() {
    int raspuns;
    printf("Vrei sa joci din nou? (1 pentru da, 0 pentru nu): ");
    scanf("%d", &raspuns);
    return raspuns;
}

void afiseazaReguli() {
    printf("\nReguli Sudoku:\n");
    printf("- Fiecare rand, coloana si subgrila 3x3 trebuie sa contina cifrele de la 1 la 9 o singura data.\n");
    printf("- Completati tabla respectand regulile de mai sus.\n\n");
}

int alegeDificultate() {
    int optiune;
    printf("Alege dificultatea:\n1. Usor\n2. Mediu\n3. Greu\n");
    scanf("%d", &optiune);
    if (optiune == 1) return 30;
    if (optiune == 2) return 40;
    return 50;
}

void afiseazaMeniu() {
    printf("\n--- MENIU SUDOKU ---\n");
    printf("1. Joaca joc nou\n");
    printf("2. Reguli joc\n");
    printf("3. Iesire\n");
}

void cereProfil() {
    printf("Numele tau: ");
    scanf("%s", jucator.nume);
    jucator.scor = 0;
}

int main() {
    srand(time(NULL));
    int optiune;
    int continuaJoc = 1;
    cereProfil();

    while (continuaJoc) {
        afiseazaMeniu();
        printf("Alege o optiune: ");
        scanf("%d", &optiune);

        if (optiune == 1) {
            int tabla[DIMENSIUNE][DIMENSIUNE];
            int original[DIMENSIUNE][DIMENSIUNE];
            jucator.scor = 0;
            genereazaSudoku(tabla);
            copiazaTabla(tabla, original);
            int dificultate = alegeDificultate();
            eliminaNumere(tabla, dificultate);
            copiazaTabla(tabla, original);
            afiseazaTabla(tabla);
            mutareUtilizator(tabla, original);
        } else if (optiune == 2) {
            afiseazaReguli();
        } else if (optiune == 3) {
            printf("La revedere!\n");
            continuaJoc = 0;
        } else {
            printf("Optiune invalida.\n");
        }
    }

    return 0;
}
