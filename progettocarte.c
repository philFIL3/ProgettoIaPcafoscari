#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAZZOCARTE 40
#define NUMEROCARTA 10
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
void effetto_carte (const Carte carta, int giocatore_corrente, int numero_giocatori, int punti_vita[], int *punti_sul_campo);

int main(void)
{
	// Carte mazzo[MAZZOCARTE]; // definisce l'array del mazzo
    Carte *mazzo = malloc(MAZZOCARTE * sizeof(Carte));
    if(mazzo == NULL) {
        return 1;
    }

// inizializza gli array di puntatori dei semi e del numero
	const char *numero_carta[] = { "Asso", "Due", "Tre", "Quattro", "Cinque", "Sei", "Sette", "Jack", "Regina", "Re"};
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
        effetto_carte(Mazzo_iterato[carte_distribuite], i, numero_giocatori, punti_vita, punti_sul_campo);
        carte_distribuite++;

         // controlla se giocatore non ha più punti vita
        if(punti_vita[i]<=0) {
            printf("Il giocatore %d è eliminato\n", i+1);
            punti_vita[i]=0;
        }
        printf("\n");
    }
}

// applica effetto delle carte
void effetto_carte(const Carte carta, int giocatore_corrente, int numero_giocatori, int punti_vita[], int *punti_sul_campo)
{
    if (strcmp(carta.numero_carta, "Due") == 0 || strcmp(carta.numero_carta, "Tre") == 0 || strcmp(carta.numero_carta, "Quattro") == 0 || strcmp(carta.numero_carta, "Cinque") == 0 || strcmp(carta.numero_carta, "Sei") == 0) {
        printf("Non succede nulla. Si procede con il prossimo turno\n");
        return;
    }

    if (strcmp(carta.numero_carta, "Sette") == 0) {
        int giocatore_successivo = (giocatore_corrente+1) % numero_giocatori;
        printf("Il giocatore %d forza il giocatore %d a scoprire ed applicare l'effetto della sua carta coperta\n", giocatore_corrente+1, giocatore_successivo+1);
    } else if (strcmp(carta.numero_carta, "Jack") == 0) {
        int giocatore_precedente = (giocatore_corrente-1+numero_giocatori) % numero_giocatori;
        punti_vita[giocatore_precedente]++;
        printf("Il giocatore %d dà 1 punto vita al giocatore %d\n", giocatore_corrente+1, giocatore_precedente+1);
    } else if (strcmp(carta.numero_carta, "Regina") == 0) {
        int giocatore_successivo = (giocatore_corrente+2) % numero_giocatori;
        punti_vita[giocatore_successivo]++;
        printf("Il giocatore %d dà 1 punto vita al giocatore %d\n", giocatore_corrente+1, giocatore_successivo+1);
    } else if (strcmp(carta.numero_carta, "Asso") == 0) {
        punti_vita[giocatore_corrente]--;
        (*punti_sul_campo)++;
        printf("Il giocatore %d perde 1 punto vita, che viene lasciato sul campo di gioco\n", giocatore_corrente+1);
        printf("Punti sul campo di gioco: %d\n", *punti_sul_campo);
    } else if (strcmp(carta.numero_carta, "Re") == 0) {
        punti_vita[giocatore_corrente] += *punti_sul_campo;
        printf("Il giocatore %d riceve %d punti vita presenti sul campo di gioco\n", giocatore_corrente+1, *punti_sul_campo);
        
        *punti_sul_campo=0; // resetta i punti sul campo
    } 
}

// scegli il primo giocatore random
int scegli_giocatore(int numero_giocatori) {
    return rand() % numero_giocatori;
}