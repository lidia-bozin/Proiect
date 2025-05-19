#include <stdio.h>
#include <stdlib.h>  
#include <time.h>   
#include "joc.h"    

int main() {
    srand(time(NULL));  

    int optiune;
    int continuaJoc = 1;

    cereProfil();

    while (continuaJoc) {
        afiseazaMeniu();
        printf("Alege o optiune: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1:
                incepeJocNou();
                break;
            case 2:
                afiseazaReguli();
                break;
            case 3:
                printf("La revedere!\n");
                continuaJoc = 0;
                break;
            case 4:
                reiaJocSalvat();
                break;
            default:
                printf("Optiune invalida.\n");
                break;
        }
    }

    return 0;
}
