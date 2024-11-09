#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

// compatibile per Mac e Windows (aggiungere...)
#ifdef _WIN32
    #include <windows.h> 
    #define CLEAR "cls"
#else // aggiungere altri se necessario
    #include <unistd.h> //mac
    #define CLEAR "clear"
#endif

#define MINGIOCATORI 2
#define MAXGIOCATORI 20
#define MAXNOME 50
#define NOME_GIOCO "Carte di Semi" // Nome del gioco
#define ANNO 2024


// ====== STRUTTURE ======
// -- Aggiungere Pannello Controllo --
// ...

typedef struct {
    char titolo[50]; //caratteri
    char opzioni[5][50]; // 5 opzioni di 50 car
    int num_opzioni;
} Menu;

typedef struct { // nome giocatore
    char nome[MAXNOME];
    char mod[20];
} Giocatore;

typedef struct {
    Giocatore *giocatori;
    int num_giocatori;
    int max_giocatori;
    int giocatori_curr;
} Lobby;

typedef struct {
    char nome[MAXNOME];
    char cognome[MAXNOME];
    char matricola[6];
} Sviluppatore;

typedef struct { // non so se è giusto
    Sviluppatore sviluppatori[4];
    int num_dev;
    char nome_progetto[MAXNOME];   
    int anno;                     
} Progetto;

typedef struct{ // da finire di implementare
    const char *numero_carta;
	const char *seme;
} Carte;


// ====== FUNZIONI UTILITY ======
// -- Schermo --
void delay(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void pulisci() {
    system(CLEAR);
}

// Sistemare a livello di layout lo schermo
void testoCentrato(const char* formato, ...) {
    char buffer[200];
    va_list args;
    va_start(args, formato);
    vsnprintf(buffer, sizeof(buffer), formato, args);
    va_end(args);

    int padding = (80 - strlen(buffer)) / 2;
    for(int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", buffer);
}

void lineaDeco() {
    for(int i = 0; i < 80; i++) printf("=");
    printf("\n");
}

void frame(char* titolo){ // - frame - 
    lineaDeco();
    testoCentrato(titolo);
    lineaDeco();
    printf("\n");
}

void pannelloControllo() { // da sistemare e implementare
    int width = 80;
    int height = 5;
    int startX = 0;
    int startY = 20;

    printf("\n");
    for(int i = 0; i < width; i++) printf("-");
    printf("\n");

    // Stampa i comandi nel pannello
    printf("Comandi: [g] Modifica numero giocatori | [m] Menu principale | [q] Esci\n");
    printf("         [r] Rimuovi giocatore        | [s] Inizia partita\n");

    // Disegna la linea inferiore del pannello
    for(int i = 0; i < width; i++) printf("-");
    printf("\n");
}

// Input Giocatore
void leggiInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Rimuove il newline
}

char leggiChar() {
    char input[3];
    leggiInput(input, sizeof(input));
    return tolower(input[0]);
}


// ====== MODALITÀ DI GIOCO ======
void modSingolo (Giocatore *giocatore) {
    char scelta;

    do {
        pulisci();
        frame("MODALITÀ: SINGOLO");

        printf("Imposta il tuo nome: ");
        leggiInput(giocatore->nome, MAXNOME);
        strcpy(giocatore->mod, "Singolo");

        pulisci();
        frame("MODALITÀ: SINGOLO");

        printf("Benvenuto %s, stai per iniziare una nuova partita di %s in %s.\n", 
               giocatore->nome, NOME_GIOCO, giocatore->mod);
        printf("Giocherai contro il Master, vuoi continuare? (y/n)");
        printf("\n\n\nm - modifica nome");
        printf("\nq - torna al menu\n\n");
        
        scelta = leggiChar();
        
        switch(scelta) {
            case 'y':
                pulisci();
                frame("PARTITA IN CORSO");
                printf("Partita %s in corso...\n", giocatore->mod);
                delay();
                printf("Prova delay");
                printf("\nq - abbandona la partita\n");
                
                while(leggiChar() != 'q') {
                    // Qui inserisci la logica del gioco
                }
                return;
                
            case 'n':
                return;
                
            case 'm':
                break;
                
            case 'q':
                return;
        }
    } while(1);
}

void modMulti() {
    Lobby lobby;
    lobby.num_giocatori = MINGIOCATORI;
    lobby.max_giocatori = MAXGIOCATORI;
    lobby.giocatori = malloc(sizeof(Giocatore) * lobby.max_giocatori);
    char scelta;
    int giocatore_corrente = 0;

    do {
        pulisci();
        frame("MODALITÀ MULTI-GIOCATORE");
        
        // Lobby
        printf("Giocatori: [%d/%d]\n", giocatore_corrente, lobby.num_giocatori);
        
        printf("\n");
        for(int i = 0; i < 80; i++) printf("_");

        for(int i = 0; i < giocatore_corrente; i++) { // giocatori inseriti
            printf("%d. %s\n", i + 1, lobby.giocatori[i].nome);
        }

        // Se non abbiamo ancora tutti i giocatori, chiedi il nome del prossimo
        if(giocatore_corrente < lobby.num_giocatori) {
            printf("\nImposta il nome del giocatore %d: ", giocatore_corrente + 1);
            leggiInput(lobby.giocatori[giocatore_corrente].nome, MAXNOME);
            strcpy(lobby.giocatori[giocatore_corrente].mod, "Multi");
            giocatore_corrente++;
        }

        pannelloControllo();

        scelta = leggiChar();
        switch(scelta) {
            case 'g':  // Modifica numero giocatori
                printf("\nInserisci il nuovo numero di giocatori (%d - %d): ", MINGIOCATORI, MAXGIOCATORI);
                char input[3];
                leggiInput(input, sizeof(input));
                int nuovo_num = atoi(input);
                if(nuovo_num >= MINGIOCATORI && nuovo_num <= MAXGIOCATORI) {
                    lobby.num_giocatori = nuovo_num;
                    if(giocatore_corrente > nuovo_num) {
                        giocatore_corrente = nuovo_num;
                    }
                }
                break;

            case 'r':  // Rimuovi ultimo giocatore
                if(giocatore_corrente > 0) {
                    giocatore_corrente--;
                    memset(lobby.giocatori[giocatore_corrente].nome, 0, MAXNOME);
                }
                break;

            case 's':  // Inizia partita
                if(giocatore_corrente == lobby.num_giocatori) {
                    pulisci();
                    frame("PARTITA IN CORSO");
                    printf("Partita Multiplayer in corso con %d giocatori...\n", lobby.num_giocatori);
                    delay();
                    free(lobby.giocatori);
                    return;
                } else {
                    printf("\nDevi prima inserire tutti i giocatori!\n");
                    delay();
                }
                break;

            case 'q':  // Esci
                free(lobby.giocatori);
                return;
        }
    } while(1);
}

// ====== GESTIONE MENU ======
int mostraMenu(Menu menu) {
    int scelta = 0;
    char input[10];

    do {
        pulisci();
        frame(menu.titolo);
        
        // Mostra opzioni
        for(int i = 0; i<menu.num_opzioni; i++){
            printf("   %d. %s\n\n", i + 1, menu.opzioni[i]);
        }

        printf("Scegli un'opzione (1-%d):", menu.num_opzioni);
        fgets(input, sizeof(input), stdin);
        scelta = atoi(input);
        
    } 
    while(scelta < 1 || scelta > menu.num_opzioni);

    return scelta;
}

void iniziaPartita() {
    char scelta;
    Giocatore giocatore = {"", ""};

    do {
        pulisci();
        frame("NUOVA PARTITA");
        printf("Scegli la modalità di gioco:\n\n");
        printf("a. Singolo\n");
        printf("b. Multi-giocatore\n");
        printf("\n\nQ - Esci\n\n");
        
        scelta = leggiChar();
        
        switch(scelta) {
            case 'a':
                modSingolo(&giocatore);
                break;
                
            case 'b':
                modMulti();
                break;
                
            case 'q':
                return;
        }
    } while(1);
}

void mostraRegole(){
    pulisci();
    frame("REGOLE DEL GIOCO");
    printf("Queste sono le regole del gioco...\n");
    // implementare regole
    getchar();
}

void mostraPunteggi(){
    pulisci();
    frame("PUNTEGGI");
    printf("Punteggi in via di sviluppo...\n");
    printf("\nPremi INVIO per tornare al menu principale...");
    // implementare punteggi
    getchar();
}

void credits() {
    Progetto progetto = {
        .sviluppatori = {
            {"Antonio", "Roncalli", "123456"},
            {"Filippo", "Pegoraro", "123455"},
            {"Melania", "Cognome", "123454"},
            {"Rachele", "Coralli", "870960"}
        },
        .num_dev = 4,
        .nome_progetto = NOME_GIOCO,
        .anno = ANNO
    };

    pulisci();
    frame("CREDITI");
    testoCentrato("Sviluppatori:\n\n");

    for (int i = 0; i < progetto.num_dev; i++) {
        testoCentrato("%s %s - %s", 
                    progetto.sviluppatori[i].nome, 
                    progetto.sviluppatori[i].cognome, 
                    progetto.sviluppatori[i].matricola);
    }

    printf("\n\n");
    testoCentrato("Progetto: %s (%d)", progetto.nome_progetto, progetto.anno);
    
    printf("\n\nPremi INVIO per tornare al menu principale...");
    getchar(); 
}
