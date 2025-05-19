#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "joc.h"
#include "util.h"

int tabla[DIMENSIUNE][DIMENSIUNE];
int solutie[DIMENSIUNE][DIMENSIUNE];
int initial[DIMENSIUNE][DIMENSIUNE];
int scor_pe_mutare = 10;
int timp_ramas = TIMP_TOTAL;
time_t start_time;

void afiseazaMeniu() {
    printf("\n===== MENIU SUDOKU =====\n");
    printf("1. Incepe joc nou\n");
    printf("2. Afiseaza regulile jocului\n");
    printf("3. Iesire\n");
    printf("4. Reia joc salvat\n");
    printf("========================\n");
}

void afiseazaReguli() {
    printf("\nReguli Sudoku:\n");
    printf("- Completati tabla de 9x9 astfel incat fiecare rand, coloana si sub-patrat 3x3 sa contina cifre de la 1 la 9 o singura data.\n");
}

void cereProfil() {
    printf("Introduceti numele jucatorului: ");
    scanf("%s", jucator.nume);
    jucator.scor = 0;
}

int esteValid(int row, int col, int num) {
    for (int x = 0; x < DIMENSIUNE; x++) {
        if (tabla[row][x] == num || tabla[x][col] == num)
            return 0;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabla[i + startRow][j + startCol] == num)
                return 0;
        }
    }
    return 1;
}

int rezolvaSudoku(int row, int col) {
    if (row == DIMENSIUNE - 1 && col == DIMENSIUNE)
        return 1;
    if (col == DIMENSIUNE) {
        row++;
        col = 0;
    }
    if (tabla[row][col] > 0)
        return rezolvaSudoku(row, col + 1);
    for (int num = 1; num <= 9; num++) {
        if (esteValid(row, col, num)) {
            tabla[row][col] = num;
            if (rezolvaSudoku(row, col + 1))
                return 1;
        }
        tabla[row][col] = 0;
    }
    return 0;
}

void copieTabla(int sursa[DIMENSIUNE][DIMENSIUNE], int destinatie[DIMENSIUNE][DIMENSIUNE]) {
    for (int i = 0; i < DIMENSIUNE; i++)
        for (int j = 0; j < DIMENSIUNE; j++)
            destinatie[i][j] = sursa[i][j];
}

void afiseazaTabla() {
    printf("\nTabla Sudoku:\n");
    for (int i = 0; i < DIMENSIUNE; i++) {
        if (i % 3 == 0 && i != 0) printf("------+-------+------\n");
        for (int j = 0; j < DIMENSIUNE; j++) {
            if (j % 3 == 0 && j != 0) printf("| ");
            printf("%d ", tabla[i][j]);
        }
        printf("\n");
    }
}

void genereazaSudoku(int dificultate) {
    memset(tabla, 0, sizeof(tabla));
    for (int i = 0; i < 11; i++) {
        int row = rand() % DIMENSIUNE;
        int col = rand() % DIMENSIUNE;
        int num = rand() % 9 + 1;
        if (esteValid(row, col, num)) {
            tabla[row][col] = num;
        }
    }
    rezolvaSudoku(0, 0);
    copieTabla(tabla, solutie);
    for (int i = 0; i < dificultate; i++) {
        int row = rand() % DIMENSIUNE;
        int col = rand() % DIMENSIUNE;
        tabla[row][col] = 0;
    }
    copieTabla(tabla, initial);
}

int alegeDificultate() {
    int optiune;
    printf("Alege dificultatea:\n");
    printf("1. Usor\n");
    printf("2. Mediu\n");
    printf("3. Greu\n");
    printf("Optiunea ta: ");
    scanf("%d", &optiune);

    switch(optiune) {
        case 1: return 30; // Usor - 30 celule goale
        case 2: return 40; // Mediu - 40 celule goale
        case 3: return 50; // Greu - 50 celule goale
        default:
            printf("Optiune invalida, se va selecta Mediu implicit.\n");
            return 40;
    }
}

void mutareUtilizator() {
    int linie, coloana, valoare;
    time_t mutare_start = time(NULL);
    printf("Introdu linie (0-8), coloana (0-8) si valoarea (1-9): ");
    scanf("%d %d %d", &linie, &coloana, &valoare);
    if (linie < 0 || linie >= DIMENSIUNE || coloana < 0 || coloana >= DIMENSIUNE || valoare < 1 || valoare > 9) {
        printf("Date invalide.\n");
        return;
    }
    if (initial[linie][coloana] == 0 && valoare == solutie[linie][coloana]) {
        tabla[linie][coloana] = valoare;
        jucator.scor += scor_pe_mutare;
        printf("Mutare corecta! Scor: %d\n", jucator.scor);
    } else {
        printf("Mutare gresita sau pozitie blocata.\n");
    }
    time_t mutare_sfarsit = time(NULL);
    timp_ramas -= (int)(mutare_sfarsit - mutare_start);
    if(timp_ramas < 0) timp_ramas = 0;
    printf("Timp ramas: %d secunde\n", timp_ramas);
}

void salveazaJoc() {
    FILE *fisier = fopen("joc_salvat.txt", "wb");
    if (fisier) {
        fwrite(tabla, sizeof(int), DIMENSIUNE * DIMENSIUNE, fisier);
        fwrite(&jucator, sizeof(struct Jucator), 1, fisier);
        fwrite(&timp_ramas, sizeof(int), 1, fisier);
        fclose(fisier);
        printf("Joc salvat cu succes.\n");
    } else {
        printf("Eroare la salvarea jocului.\n");
    }
}

void incarcaJoc() {
    FILE *fisier = fopen("joc_salvat.txt", "rb");
    if (fisier) {
        fread(tabla, sizeof(int), DIMENSIUNE * DIMENSIUNE, fisier);
        fread(&jucator, sizeof(struct Jucator), 1, fisier);
        fread(&timp_ramas, sizeof(int), 1, fisier);
        fclose(fisier);
        copieTabla(tabla, initial);
        printf("Joc incarcat. Bine ai revenit, %s!\n", jucator.nume);
    } else {
        printf("Nu exista joc salvat.\n");
    }
}

int raspunsDa() {
    char raspuns[10];
    scanf("%9s", raspuns);
    return strcmp(raspuns, "da") == 0;
}

void incepeJocNou() {
    int dificultate = alegeDificultate();
    genereazaSudoku(dificultate);
    start_time = time(NULL);
    timp_ramas = TIMP_TOTAL;
    cereProfil();
    while (timp_ramas > 0) {
        afiseazaTabla();
        mutareUtilizator();

        printf("Vrei sa salvezi jocul? (da/nu): ");
        if (raspunsDa()) {
            salveazaJoc();
        }

        printf("Vrei sa iesi din joc? (da/nu): ");
        if (raspunsDa()) {
            printf("Iesire din joc.\n");
            break;
        }
    }
    printf("Timpul a expirat! Scor final: %d\n", jucator.scor);
    double procentaj = (double)jucator.scor / (DIMENSIUNE * DIMENSIUNE * scor_pe_mutare) * 100;
    printf("Progres completat: %.2f%%\n", procentaj);
}

void reiaJocSalvat() {
    incarcaJoc();
    start_time = time(NULL);
    while (timp_ramas > 0) {
        afiseazaTabla();
        mutareUtilizator();

        printf("Vrei sa salvezi jocul? (da/nu): ");
        if (raspunsDa()) {
            salveazaJoc();
        }

        printf("Vrei sa iesi din joc? (da/nu): ");
        if (raspunsDa()) {
            printf("Iesire din joc.\n");
            break;
        }
    }
    printf("Timpul a expirat! Scor final: %d\n", jucator.scor);
    double procentaj = (double)jucator.scor / (DIMENSIUNE * DIMENSIUNE * scor_pe_mutare) * 100;
    printf("Progres completat: %.2f%%\n", procentaj);
}
