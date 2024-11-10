#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAZZOCARTE 52
#define NUMEROCARTA 13
#define MINGIOCATORI 2
#define MAXGIOCATORI 20
#define PUNTIVITA 2


// definizione struttura carta
struct carte {
	const char *numero_carta;
	const char *seme;
};
typedef struct carte Carte; // nuovo nome di tipo per struct carte

// prototipi
void new_mazzo(Carte * const Mazzo_iterato, const char * Array_numero_carta[],
               const char * Array_Seme[]);
void mischia_mazzo(Carte * const Mazzo_iterato);
void distribuisci(const Carte * const Mazzo_iterato, int numero_giocatori, int punti_vita[], int *punti_sul_campo);   
int scegli_giocatore(int numero_giocatori); 

int main(void)
{
	// Carte mazzo[MAZZOCARTE]; // definisce l'array del mazzo
    Carte *mazzo = malloc(MAZZOCARTE * sizeof(Carte));
    if(mazzo == NULL) {
        return 1;
    }

// inizializza gli array di puntatori dei semi e del numero
	const char *numero_carta[] = { "Asso", "Due", "Tre", "Quattro", "Cinque", "Sei", "Sette", "Otto", "Nove", "Dieci", "Jack", "Regina", "Re"};
	const char *seme[] = {"Picche", "Quadri", "Cuori", "Fiori"};

	srand(time(NULL)); /* se usassimo rand() la combinazione sarC  la stessa per ogni iterazione, con srand inizializziamo una iterazione randomica unica ogni qual volta che avviamo il programma, (time(NULL) serve per randomizzare l'iterazione di srand()*/

	new_mazzo(mazzo, numero_carta, seme); // crea il mazzo di carte
	mischia_mazzo(mazzo); // mischia le carte del mazzo

    int numero_carte=MAZZOCARTE;
    int numero_giocatori, carte_giocatori;

    // chiede il numero di giocatori
    printf("Inserire il numero di giocatori (da %d a %d): ", MINGIOCATORI, MAXGIOCATORI);
    scanf("%d", &numero_giocatori);
    printf("\n");

    if(numero_giocatori<MINGIOCATORI || numero_giocatori>MAXGIOCATORI) {
        printf("Il numero di giocatori non è valido!!\n");
        free(mazzo); 
        return 1;
    }

    // assegna 2 punti vita iniziali ai giocatori
    int punti_vita [MAXGIOCATORI];
    for(int i=0; i<numero_giocatori; i++) {
        punti_vita[i]=PUNTIVITA;
    }
    
    int punti_sul_campo=0;  // tiene conto dei punti sul campo
    distribuisci(mazzo, numero_giocatori, punti_vita, &punti_sul_campo); // distribuisci le carte

    // sceglie il primo giocatore random
    int primo_giocatore = scegli_giocatore(numero_giocatori);
    printf("Il primo giocatore è il giocatore %d\n", primo_giocatore+1);

    free(mazzo);
    mazzo=NULL; // azzera il puntatore

    return 0;
}

// creazione mazzo di carte 
void new_mazzo(Carte * const Mazzo_iterato, const char * Array_numero_carta[],
               const char * Array_Seme[])
{
	// size_t nuovo_mazzo;
	for (size_t mazzo_nuovo = 0; mazzo_nuovo < MAZZOCARTE; ++mazzo_nuovo) {
		Mazzo_iterato[mazzo_nuovo].numero_carta = Array_numero_carta[mazzo_nuovo % NUMEROCARTA];
		Mazzo_iterato[mazzo_nuovo].seme = Array_Seme[mazzo_nuovo / NUMEROCARTA];
// Mazzo_iterato[mazzo_nuovo] accede agli elementi di .seme e .numero_carta
	}
}
// mischia le carte
void mischia_mazzo(Carte * const Mazzo_iterato)
{
	for (size_t i = 0; i < MAZZOCARTE; ++i) {
		size_t j = rand() % MAZZOCARTE ;
		Carte randomico = Mazzo_iterato[i];
		Mazzo_iterato[i] = Mazzo_iterato[j];
		Mazzo_iterato[j] = randomico;
	}
}

// distribuisci le carte
void distribuisci(const Carte * const Mazzo_iterato, int numero_giocatori, int punti_vita[], int *punti_sul_campo)
{
    int carte_distribuite = 0;

    for (int i = 0; i < numero_giocatori; ++i){
        printf("Il giocatore %d ha %d punti vita iniziali e riceve: \n", i+1, punti_vita[i]);
        printf("Carta coperta: %s di %s\n", Mazzo_iterato[carte_distribuite].numero_carta, Mazzo_iterato[carte_distribuite].seme);
        carte_distribuite++;
        printf("Carta scoperta: %s di %s\n", Mazzo_iterato[carte_distribuite].numero_carta, Mazzo_iterato[carte_distribuite].seme);
        carte_distribuite++;
        printf("\n");
    }
// to do, ridurre il numero di carte nell'array in base ai numeri di giocatori
//cancellare poi il mazzo per risparmiare memoria
	/*size_t i;
	for (i = 0; i < 2; ++i) {
		printf("%s di %s\n", Mazzo_iterato[i].numero_carta, Mazzo_iterato[i].seme);
	}*/
}

// scegli il primo giocatore random
int scegli_giocatore(int numero_giocatori) {
    return rand() % numero_giocatori;
}