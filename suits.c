#include <locale.h>
#include <stdio.h>
#include <wchar.h>

int main(int argc, char* argv[])
{
        setlocale(LC_ALL, "");

        wchar_t *clubs = L"\u2663  \u2667"; //la prima da il colore pieno, la seconda solo il contorno
        wchar_t *diamonds = L"\u2666  \u2662";
        wchar_t *hearts = L"\u2665  \u2661";
        wchar_t *spades = L"\u2660  \u2664";

        printf("%ls\n", clubs);
        printf("%ls\n", diamonds);
        printf("%ls\n", hearts);
        printf("%ls\n", spades);

        return 0;
}
