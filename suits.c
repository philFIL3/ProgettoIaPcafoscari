#include <locale.h>
#include <stdio.h>
#include <wchar.h>

int suits(int argc, char* argv[])
{
        setlocale(LC_ALL, "");

        wchar_t *clubs = L"\u2663  \u2667";
        wchar_t *diamonds = L"\u2666  \u2662";
        wchar_t *hearts = L"\u2665  \u2661";
        wchar_t *spades = L"\u2660  \u2664";

        printf("%ls\n", clubs);         // fiori
        printf("%ls\n", diamonds);      // quadri
        printf("%ls\n", hearts);        // cuori
        printf("%ls\n", spades);        // picche

        return 0;
}

int semi(int argc, char* argv[]) {
        
        setlocale (LC_ALL, "");

        wchar_t *cuori = L"\u2665";
        wchar_t *quadri = L"\u2666";
        wchar_t *picche = L"\u2660"; 
        wchar_t *fiori = L"\u2663";

        printf("%ls\n", cuori);
        printf("%ls\n", quadri);
        printf("%ls\n", picche);
        printf("%ls\n", fiori); 
        
        return 0;
}

int main (int argc, char* argv[]){

        semi(argc, argv);
        return 0;
}
