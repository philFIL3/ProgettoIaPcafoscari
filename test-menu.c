#include <stdio.h>
#include <stdlib.h>

#define MINGIOCATORI 2
#define MAXGIOCATORI 4

void mostraMenu();
void gioca();
void selNumGiocatori();

int main() {
    int scelta = 0;

    while (1) {
        mostraMenu();
        printf("\nScegli un'opzione: ");
        scanf("%d", &scelta);

        switch (scelta) {
            case 1:
                gioca(); // da implementare con il codice delle carte assegnate
                break;
            case 2:
                selNumGiocatori();
                break;
            case 0:
                printf("Uscita dal gioco. Arrivederci!\n");
                exit(0);
            default:
                printf("Scelta non valida. Riprova.\n");
        }
    }

    return 0;
}

void mostraMenu() { // magari cambiare il nome in Home / menuHome / menuStart
    system("clear");  // "cls" per Windows
    printf("=== Carte Semi ===\n\n");
    printf("1. Gioca\n");
    printf("2. Giocatori\n");
    printf("0. Esci\n");
} // al posto di numeri non so se convenga mettere (come nei giochi vecchi) lettere tipo: 
// P (play): gioca / o (g)
// G (giocatori) / o / N (numero giocatori)
// E (exit, esci)

void gioca() {
    printf("Inizia il gioco...\n");
    // Da implementare con le carte assegnate del codice gia fatto (pensare a grafica)
    // qui si blocca, non capisco
}

void selNumGiocatori() {
    int numeroGiocatori;
    printf("Seleziona il numero di giocatori (da %d a %d): ", MINGIOCATORI, MAXGIOCATORI);
    scanf("%d", &numeroGiocatori);
    if (numeroGiocatori >= 2 && numeroGiocatori <= 4) { // non so se è giusto
        printf("Hai selezionato %d giocatori.\n", numeroGiocatori);
    } else {
        printf("Numero di giocatori non valido. Riprova.\n");
    } // qui dovrebbe tornare indietro e farmi riprovare, ma non va
}
