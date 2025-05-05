#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DIMENSIUNE 9
#define TIMP_TOTAL 1200  // 20 minute

struct Jucator {
    char nume[50];
    int scor;
};
struct Jucator jucator;

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

void afiseazaTimpRamas(time_t start) {
    time_t acum = time(NULL);
    int timp_ramas = TIMP_TOTAL - (int)(acum - start);
    if (timp_ramas < 0) timp_ramas = 0;
    int minute = timp_ramas / 60;
    int secunde = timp_ramas % 60;
    printf("\u23F3 Timp ramas: %02d:%02d\n", minute, secunde);
}

void punctajPeMutare(int scorAdaugat) {
    printf("+%d puncte! Scor curent: %d\n", scorAdaugat, jucator.scor);
}

void afiseazaScorFinal(int scorMaxim) {
    float procentaj = ((float)jucator.scor / scorMaxim) * 100;
    printf("\nScor final: %d / %d (%.2f%%)\n", jucator.scor, scorMaxim, procentaj);
    if (procentaj >= 90)
        printf("MAESTRU SUDOKU, %s!\n", jucator.nume);
    else if (procentaj >= 70)
        printf("BINE JUCAT, %s!\n", jucator.nume);
    else
        printf("MAI \xCENCEARC\xC4, %s!\n", jucator.nume);
}

void salveazaJoc(int tabla[DIMENSIUNE][DIMENSIUNE], int original[DIMENSIUNE][DIMENSIUNE], time_t start) {
    FILE *fisier = fopen("C:/calea/unde/vrei/joc_salvat.txt", "w");
    if (fisier == NULL) {
        printf("Eroare la salvarea jocului!\n");
        return;
    }
    fprintf(fisier, "%s\n%d\n%ld\n", jucator.nume, jucator.scor, start);
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            fprintf(fisier, "%d ", tabla[i][j]);
    fprintf(fisier, "\n");
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            fprintf(fisier, "%d ", original[i][j]);
    fclose(fisier);
    printf("✅ Jocul a fost salvat cu succes.\n");
}

int incarcaJoc(int tabla[DIMENSIUNE][DIMENSIUNE], int original[DIMENSIUNE][DIMENSIUNE], time_t *start) {
    FILE *fisier = fopen("C:/calea/unde/vrei/joc_salvat.txt", "r");
    if (fisier == NULL) {
        printf("Nu exista niciun joc salvat.\n");
        return 0;
    }
    fscanf(fisier, "%s", jucator.nume);
    fscanf(fisier, "%d", &jucator.scor);
    fscanf(fisier, "%ld", start);
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            fscanf(fisier, "%d", &tabla[i][j]);
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            fscanf(fisier, "%d", &original[i][j]);
    fclose(fisier);
    printf("✅ Joc salvat incarcat cu succes!\n");
    return 1;
}

void mutareUtilizator(int tabla[DIMENSIUNE][DIMENSIUNE], int original[DIMENSIUNE][DIMENSIUNE], time_t start) {
    int rand, col, numar;
    int scorMaxim = 0;
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            if (original[i][j] == 0)
                scorMaxim += 10;
    while (!tablaPlina(tabla)) {
        afiseazaTimpRamas(start);
        if (time(NULL) - start >= TIMP_TOTAL) {
            printf("\u23F1️ Timpul a expirat! Jocul s-a incheiat.\n");
            break;
        }
        printf("Introduceti rand (1-9), coloana (1-9) si numarul (1-9), sau 'iesire' / 'salveaza':\n");
        char comanda[10];
        scanf("%s", comanda);
        if (strcmp(comanda, "iesire") == 0) {
            printf("Ai ales să ieși din joc. La revedere!\n");
            break;
        } else if (strcmp(comanda, "salveaza") == 0) {
            salveazaJoc(tabla, original, start);
            continue;
        }
        rand = atoi(comanda) - 1;
        scanf("%d %d", &col, &numar);
        col--;
        if (original[rand][col] == 0 && mutareValida(tabla, rand, col, numar)) {
            tabla[rand][col] = numar;
            jucator.scor += 10;
            punctajPeMutare(10);
            afiseazaTabla(tabla);
        } else {
            printf("MUTARE INVALIDĂ! MAI ÎNCEARCĂ!\n");
        }
    }
    if (tablaPlina(tabla) && tablaCorecta(tabla)) {
        printf("FELICITĂRI! AI COMPLETAT COMPLET SUDOKU!\n");
    } else {
        printf("SUDOKU INCOMPLET.\n");
    }
    afiseazaScorFinal(scorMaxim);
}

int alegeDificultate() {
    int optiune;
    printf("Alege dificultatea:\n1. Ușor\n2. Mediu\n3. Greu\n");
    scanf("%d", &optiune);
    if (optiune == 1) return 30;
    if (optiune == 2) return 40;
    return 50;
}

void afiseazaMeniu() {
    printf("\n--- MENIU SUDOKU ---\n");
    printf("1. Joacă joc nou\n");
    printf("2. Reguli joc\n");
    printf("3. Ieșire\n");
    printf("4. Reia joc salvat\n");
}

void afiseazaReguli() {
    printf("\nReguli Sudoku:\n");
    printf("- Fiecare rand, coloana si subgrila 3x3 trebuie sa contina cifrele de la 1 la 9 o singura data.\n");
    printf("- Completati tabla respectand regulile de mai sus.\n\n");
    printf("- Completati sudoku la timp.\n\n");
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
            time_t start = time(NULL);
            mutareUtilizator(tabla, original, start);
        } else if (optiune == 2) {
            afiseazaReguli();
        } else if (optiune == 3) {
            printf("La revedere!\n");
            continuaJoc = 0;
        } else if (optiune == 4) {
            int tabla[DIMENSIUNE][DIMENSIUNE];
            int original[DIMENSIUNE][DIMENSIUNE];
            time_t start;
            if (incarcaJoc(tabla, original, &start)) {
                afiseazaTabla(tabla);
                mutareUtilizator(tabla, original, start);
            }
        } else {
            printf("Optiune invalida.\n");
        }
    }
    return 0;
}