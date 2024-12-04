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
void new_mazzo(Carte * const Mazzo_iterato, const char * Array_numero_carta[], const char * Array_Seme[]);
void mischia_mazzo(Carte * const Mazzo_iterato);
void distribuisci(const Carte * const Mazzo_iterato, int numero_giocatori, int punti_vita[], int *punti_sul_campo, int prima_mano, Carte carte_scoperte[]);   
int scegli_giocatore(int numero_giocatori); 
void effetto_carte (const Carte carta, int giocatore_corrente, int numero_giocatori, int punti_vita[], int *punti_sul_campo);
void fasi(Carte *const mazzo, int numero_giocatori, int punti_vita[], int *punti_sul_campo, Carte carte_scoperte[]);

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

    // sceglie il primo giocatore random
    int primo_giocatore = scegli_giocatore(numero_giocatori);
    printf("Il primo giocatore è il giocatore %d\n", primo_giocatore+1);

    // assegna 2 punti vita iniziali ai giocatori
    int punti_vita [MAXGIOCATORI];
    for(int i=0; i<numero_giocatori; i++) {
        punti_vita[i]=PUNTIVITA;
    }
    
    int punti_sul_campo=0;  // tiene conto dei punti sul campo
    Carte carte_scoperte[MAXGIOCATORI];  // salva le carte scoperte
    distribuisci(mazzo, numero_giocatori, punti_vita, &punti_sul_campo, 1, carte_scoperte); // distribuisce le carte

    fasi(mazzo, numero_giocatori, punti_vita, &punti_sul_campo, carte_scoperte);  // inizia le fasi

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
void distribuisci(const Carte * const Mazzo_iterato, int numero_giocatori, int punti_vita[], int *punti_sul_campo, int prima_mano, Carte carte_scoperte[])
{
    int carte_distribuite = 0;

    for (int i = 0; i < numero_giocatori; ++i){
        printf("\nIl giocatore %d ha %d punti vita iniziali e riceve: \n", i+1, punti_vita[i]);
        printf("Carta coperta: %s di %s\n", Mazzo_iterato[carte_distribuite].numero_carta, Mazzo_iterato[carte_distribuite].seme);
        carte_distribuite++;

        carte_scoperte[i] = Mazzo_iterato[carte_distribuite];
        printf("Carta scoperta: %s di %s\n", Mazzo_iterato[carte_distribuite].numero_carta, Mazzo_iterato[carte_distribuite].seme);
        
        // fa applicare l'effetto delle carte solo se non è la prima mano
        if(!prima_mano){
            effetto_carte(Mazzo_iterato[carte_distribuite], i, numero_giocatori, punti_vita, punti_sul_campo);
        }

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
        printf("Non succede nulla\n");
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
         // distingue i casi in cui ho e non ho punti sul campo 
        if (*punti_sul_campo<=0) {
            printf("Non ci sono punti sul campo di gioco, quindi il giocatore %d non riceve alcun punto\n", giocatore_corrente+1);
        } else if (*punti_sul_campo==1) {  // caso in cui ce n'è solo uno
           printf("Il giocatore %d riceve %d punto vita presente sul campo di gioco\n", giocatore_corrente+1, *punti_sul_campo); 
        } else if (*punti_sul_campo>1) {
            printf("Il giocatore %d riceve %d punti vita presenti sul campo di gioco\n", giocatore_corrente+1, *punti_sul_campo);
        }
        
        *punti_sul_campo=0; // resetta i punti sul campo
    } 
}

// scegli il primo giocatore random
int scegli_giocatore(int numero_giocatori) {
    return rand() % numero_giocatori;
}

// definisce le fasi del gioco
void fasi(Carte *const mazzo, int numero_giocatori, int punti_vita[], int *punti_sul_campo, Carte carte_scoperte[]) 
{
    int turno_corrente = 0;
    int fase_corrente = 1;
    int giocatori_attivi = numero_giocatori;

    printf("Inizio della partita!!\n");

    while(giocatori_attivi>1) {
        printf("\n\nFase %d\n", fase_corrente);

        for(int i=0; i<numero_giocatori; i++) {
            // passa al giocatore successivo
            while(punti_vita[turno_corrente]<=0) {
                turno_corrente = (turno_corrente+1)%numero_giocatori;
            }
            printf("\nTurno del giocatore %d\n", turno_corrente+1);

            printf("Il giocatore %d applica l'effetto della carta scoperta: %s di %s\n", turno_corrente+1, carte_scoperte[turno_corrente].numero_carta, carte_scoperte[turno_corrente].seme);
            effetto_carte(carte_scoperte[turno_corrente], turno_corrente, numero_giocatori, punti_vita, punti_sul_campo);

            // verifica se il giocatore corrente è eliminato
            if(punti_vita[turno_corrente]<=0) {
                printf("Il giocatore %d è eliminato\n", turno_corrente+1);
                punti_vita[turno_corrente]=0;
                giocatori_attivi--;
                continue;
            }

            printf("La tua carta coperta è: %s di %s\n", mazzo[turno_corrente*2].numero_carta, mazzo[turno_corrente*2].seme);
            printf("Vuoi scoprire la tua carta coperta? ");
            char scelta[3];
            scanf("%2s", scelta);

            if(strcmp(scelta, "si")==0 || strcmp(scelta, "Si")==0 || strcmp(scelta, "sì")==0 || strcmp(scelta, "Sì")==0) {
                printf("\nIl giocatore %d scopre la carta %s di %s e ne applica l'effetto\n", turno_corrente+1, mazzo[turno_corrente*2].numero_carta, mazzo[turno_corrente*2].seme);
                effetto_carte(mazzo[turno_corrente*2], turno_corrente, numero_giocatori, punti_vita, punti_sul_campo);
            } else if(strcmp(scelta, "no")==0 || strcmp(scelta, "No") ==0) {
                printf("Il giocatore %d decide di non scoprire la sua carta coperta\n", turno_corrente+1);
            } else {
                printf("Scelta non valida\n");
            }

            do {
                turno_corrente=(turno_corrente+1)%numero_giocatori;
            } while (punti_vita[turno_corrente]<=0);
            
            // controlla se ci sono dei giocatori di eliminare alla fine della fase
            printf("\nFine della fase %d\n", fase_corrente);
            giocatori_attivi=0;
            for(int i=0; i<numero_giocatori; i++) {
                if(punti_vita[i]>0) {
                    printf("Il giocatore %d ha ancora %d punti vita\n", i+1, punti_vita[i]);
                    giocatori_attivi++;
                } else {
                    printf("Il giocatore %d ha terminato i punti vita ed è eliminato\n", i+1);
                }
            }
        }
        fase_corrente++;
    }
    printf("\nPartita terminata!!\n");
    for(int i=0; i<numero_giocatori; i++) {
        if(punti_vita[i]>0) {
            printf("Ha vinto il giocatore %d\n", i+1);
            break;
        }
    }
}