#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

void meni_glavni(Komponenta** skladiste);
void meni_admin(Komponenta** glava);

int main(void) {
    Komponenta* skladiste = ucitaj_komponente_iz_datoteke("komponente.bin");

    meni_glavni(&skladiste);

    if (!sacuvaj_komponente_u_datoteci("komponente.bin", skladiste)) {
        printf("Greška pri spremanju komponenti.\n");
    }

    oslobodi_komponente(skladiste);
    return 0;
}

void meni_glavni(Komponenta** skladiste) {
    int izbor;
    bool kraj = false;

    while (!kraj) {
        printf("\n--- Glavni izbornik ---\n");
        printf("%d. Administracija\n", IZBORNIK_ADMIN);
        printf("%d. Korisnicki izbornik \n", IZBORNIK_KORISNIK);
        printf("%d. Izlaz\n", IZBORNIK_IZLAZ);
        izbor = ucitaj_int("Odaberite opciju: ");

        switch (izbor) {
        case IZBORNIK_ADMIN:
            meni_admin(skladiste);
            break;
        case IZBORNIK_KORISNIK:
            meni_korisnik(*skladiste);
            break;
        case IZBORNIK_IZLAZ:
            kraj = true;
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    }
}
