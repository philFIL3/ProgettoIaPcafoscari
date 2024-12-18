#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h> // Per setlocale
#include <wchar.h>  // Per wchar_t

// Compatibilità windows e mac
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
    //aggiungere se necessario
#else
    #include <unistd.h>
    #define CLEAR "clear"
    //aggiungere altri..
#endif

// --- COSTANTI --- 
#define MAZZOCARTE 40
#define NUMEROCARTA 10
#define MINGIOCATORI 2
#define MAXGIOCATORI 20
#define PUNTIVITA 2

//--- UTILITY ---
void pulisci(){ // pulisci schermo 
    system(CLEAR);
}

void delay() { // funzione di delay per simulare caricamenti (int ms)
#ifdef _WIN32
    Sleep(900); 
#else
    usleep(900000);
#endif
}

//--- STRUTTURE DATI --- 
typedef enum {
    FIORI, CUORI, QUADRI, PICCHE,
} Seme;

typedef enum {
    ASSO = 1, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, JACK, REGINA, RE
} TipoCarta;

typedef enum {
    NULLO,      // nessun effetto
    MAGIA,      // scopre la carta avversario
    SUPPORTO,   // donare hp
    TRAPPOLA,   // perde 1 hp sul campo
    LADRO       // sottrae risorse
    //SPECIALE, SCARTO...
    
} CategEffetto; // categoria di ogni effetto

typedef struct {
    CategEffetto categoria;
    void (*effetto)(int giocatore_corrente, int giocatore_target);
} Effetto;

typedef struct {
    TipoCarta tipo;  // tipo della carta
    Seme seme;      // uno dei quattro semi
    int valore_carta; // valore effettivo della carta
    Effetto* effetto;
} Carta; 

typedef struct {
    char id[MAXGIOCATORI];  // ?[non so se giusto] ID del giocatore, massimo fino a maxgiocatori(20)
    int punti_vita;         // Punti vita attuali
    Carta mano[2];      // Carte in mano (esempio con massimo 5 carte)
    Carta coperta;
    Carta scoperta;
} Giocatore;

typedef struct {
    int num_giocatori;     // Numero di giocatori attivi
    int punti_campo;       // Punti vita lasciati in campo
    Giocatore giocatori[MAXGIOCATORI]; // Array di giocatori
    Carta mazzo[MAZZOCARTE];  // guardare se modificare -> stato sulle carte disponibili
} StatoGioco;


//--- Funzioni ---
void crea_mazzo(Carta mazzo[MAZZOCARTE]){
    int i = 0;
    for (int i_seme = FIORI; i_seme <= PICCHE; i_seme++){
        for(int i_valore = ASSO; i_valore <= RE; i_valore++){
            mazzo[i].tipo = i_valore; // tipo della carta (nome)
            mazzo[i].seme = i_seme; // seme della carta
            mazzo[i].valore_carta = i_valore; // valore della carta(1, 2, 3...)
            i++;
        }
    }
}

// simboli per ogni seme (con puntatore a stringa wchar_t)
const wchar_t* simboli_semi(Seme seme){ //stampa con %ls - wprintf
    setlocale(LC_ALL, ""); // per i caratteri unicode
    
    switch(seme) {
        case FIORI: return L"\u2663";  // ♣
        case CUORI: return L"\u2665";  // ♥
        case QUADRI: return L"\u2666"; // ♦
        case PICCHE: return L"\u2660"; // ♠
        default: return L"?";    
    }
}

const wchar_t* nomi_carta(TipoCarta tipo) {
    switch (tipo) {
        case ASSO: return L"A";
        case JACK: return L"J";
        case REGINA: return L"Q";
        case RE: return L"K";
        default: {
            static wchar_t buffer[3];
            swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%d", tipo);
            return buffer;
        }
    }
}

// --- PROTOTIPI --- 
void inizia_partita();
void crea_mazzo(Carta mazzo[MAZZOCARTE]);
void mescola_mazzo(Carta mazzo[MAZZOCARTE]);
void distribuisci_carte(Carta mazzo[MAZZOCARTE], Giocatore giocatori[], int num_giocatori);

//--- Funzioni ---
void crea_mazzo(Carta mazzo[MAZZOCARTE]){
    int i = 0;
    for (int i_seme = FIORI; i_seme <= PICCHE; i_seme++){
        for(int i_valore = ASSO; i_valore <= RE; i_valore++){

            //se vogliamo assegnare il valore di J Q K a 8 (es. se con mana ecc, allora mettere if...)
            //stessa cosa se vogliamo che A = 1 o 11
            mazzo[i].tipo = i_valore; // tipo della carta (valore)
            mazzo[i].seme = i_seme; // seme della carta
            mazzo[i].valore_carta = i_valore; // valore della carta(1, 2, 3...)
            i++;

            // Allocazione e associazione degli effetti
            Effetto *effetto_carta = malloc(sizeof(Effetto));
            if (!effetto_carta) {
                perror("Errore nell'allocazione di Effetto");
                exit(EXIT_FAILURE);
            }

            switch (i_valore) {
                case SETTE:
                    effetto_carta->categoria = MAGIA;
                    effetto_carta->effetto = effetto_scoperta;
                    break;
                case JACK:
                    effetto_carta->categoria = SUPPORTO;
                    effetto_carta->effetto = effetto_vita;
                    break;
                case REGINA:
                    effetto_carta->categoria = SUPPORTO;
                    effetto_carta->effetto = effetto_vita2;
                    break;
                case ASSO:
                    effetto_carta->categoria = TRAPPOLA;
                    effetto_carta->effetto = effetto_trappola;
                    break;
                case RE:
                    effetto_carta->categoria = LADRO;
                    effetto_carta->effetto = effetto_ladro;
                    break;
                default:
                    effetto_carta->categoria = NULLO;
                    effetto_carta->effetto = effetto_nullo;
                    break;
            }

            mazzo[i].effetto = effetto_carta; // carta associata all'effetto
            i++;
        }
    }
}

//importante per liberare memoria 
void libera_mazzo(Carta mazzo[MAZZOCARTE]) {
    for (int i = 0; i < MAZZOCARTE; i++) {
        if (mazzo[i].effetto) {
            free(mazzo[i].effetto);
            mazzo[i].effetto = NULL;
        }
    }
}

void mostra_mazzo(Carta mazzo[MAZZOCARTE]){
    for (int i = 0; i < MAZZOCARTE; i++){
        wprintf(L"Carta: %ls %ls (valore: %d)\n",
        nomi_carta(mazzo[i].tipo), simboli_semi(mazzo[i].seme), mazzo[i].valore_carta);
    }
}

void mescola_mazzo(Carta mazzo[MAZZOCARTE]){ 
    srand(time(NULL)); // generazione casuale
    for(int i = 0; i < MAZZOCARTE; i++){ // 0 - MAZZOCARTE-1
        int rand_i = rand() % MAZZOCARTE; 
        Carta randomica = mazzo[i]; //la carta corrente viene spostata con una carta randomica (time null)
        mazzo[i] = mazzo[rand_i];
        mazzo[rand_i] = randomica;
    }
}

// -- EFFETTI CARTE -- 
void effetto_nullo(int giocatore_corrente, int giocatore_target) {
    printf("Non succede nulla.\n");
}

void effetto_scoperta(int giocatore_corrente, StatoGioco *stato) { //giocatore successivo (+1)
    int giocatore_successivo = (giocatore_corrente + 1) % stato->num_giocatori;
    printf("Il giocatore %s forza il giocatore %s a scoprire e applicare l'effetto della sua carta coperta.\n", 
           stato->giocatori[giocatore_corrente].id, stato->giocatori[giocatore_successivo].id);
    attiva_effetto(stato->giocatori[giocatore_successivo].coperta, stato, giocatore_successivo, giocatore_successivo);
}

void effetto_vita(int giocatore_corrente, StatoGioco *stato) { //giocatore precedente (-1)
    int giocatore_precedente = (giocatore_corrente - 1 + stato->num_giocatori) % stato->num_giocatori;
    stato->giocatori[giocatore_precedente].punti_vita++;
    printf("Il giocatore %s dà 1 punto vita al giocatore %s.\n", 
           stato->giocatori[giocatore_corrente].id, stato->giocatori[giocatore_precedente].id);
}

void effetto_vita2(int giocatore_corrente, StatoGioco *stato) { // 2 giocatori successivi
    int giocatore_successivo = (giocatore_corrente + 2) % stato->num_giocatori;
    stato->giocatori[giocatore_successivo].punti_vita++;
    printf("Il giocatore %s dà 1 punto vita al giocatore %s.\n", 
           stato->giocatori[giocatore_corrente].id, stato->giocatori[giocatore_successivo].id);
}

void effetto_trappola(int giocatore_corrente, StatoGioco *stato) { // inganno
    stato->giocatori[giocatore_corrente].punti_vita--;
    stato->punti_campo++;
    printf("Il giocatore %s perde 1 punto vita, che viene lasciato sul campo di gioco.\n", 
           stato->giocatori[giocatore_corrente].id);
    printf("Punti sul campo di gioco: %d\n", stato->punti_campo);
}

void effetto_ladro(int giocatore_corrente, StatoGioco *stato) { // piglia tutto
    stato->giocatori[giocatore_corrente].punti_vita += stato->punti_campo;

    if (stato->punti_campo == 0) {
        printf("Non ci sono punti sul campo di gioco, quindi il giocatore %s non riceve alcun punto.\n", 
               stato->giocatori[giocatore_corrente].id);
    } else {
        printf("Il giocatore %s riceve %d punti vita presenti sul campo di gioco.\n", 
               stato->giocatori[giocatore_corrente].id, stato->punti_campo);
    }

    stato->punti_campo = 0; // Reset punti sul campo
}

void attiva_effetto(Carta carta, StatoGioco *stato, int giocatore_corrente, int giocatore_target) {
    if (carta.effetto && carta.effetto->effetto != NULL) {
        carta.effetto->effetto(giocatore_corrente, stato);
    }
}

void stampa_mazzo(){
    Carta mazzo[MAZZOCARTE];    // inizia il mazzo carte (40)
    crea_mazzo(mazzo);          // creo il mazzo di mazzo
    mostra_mazzo(mazzo);         //mostro il mazzo (stampando)
}



int main(){
    pulisci();
    stampa_mazzo();
    return 0;
}
