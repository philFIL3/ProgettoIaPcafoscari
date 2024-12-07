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
#define PUNTIVITA 10

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
} NomeCarta;

typedef struct {
    NomeCarta nome;  // nome della carta
    Seme seme;   // uno dei quattro semi
    int valore_carta; // valore effettivo della carta
} Carta; 


//--- Funzioni ---
void crea_mazzo(Carta mazzo[MAZZOCARTE]){
    int i = 0;
    for (int i_seme = FIORI; i_seme <= PICCHE; i_seme++){
        for(int i_valore = ASSO; i_valore <= RE; i_valore++){
            mazzo[i].nome = i_valore; // nome della carta (valore)
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

const wchar_t* nomi_carta(NomeCarta nome) {
    switch (nome) {
        case ASSO: return L"A";
        case JACK: return L"J";
        case REGINA: return L"Q";
        case RE: return L"K";
        default: {
            static wchar_t buffer[3];
            swprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), L"%d", nome);
            return buffer;
        }
    }
}
void mostra_mazzo(Carta mazzo[MAZZOCARTE]){
    for (int i = 0; i < MAZZOCARTE; i++){
        wprintf(L"Carta: %ls %ls (valore: %d)\n",
        nomi_carta(mazzo[i].nome), simboli_semi(mazzo[i].seme), mazzo[i].valore_carta);
    }
}

void stampa_mazzo(){
    Carta mazzo[MAZZOCARTE];    // inzia il mazzo carte (40)
    crea_mazzo(mazzo);          // creo il mazzo di mazzo
    mostra_mazzo(mazzo);         //mostro il mazzo (stampando)
}

int main(){
    pulisci();
    stampa_mazzo();
    return 0;
}
