#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

bool provjeri_lozinku_admina(const char* unos) {
    return strcmp(unos, ADMIN_LOZINKA) == 0;
}

Komponenta* kreiraj_novu_komponentu(Komponenta* glava) {
    Komponenta* nova = (Komponenta*)malloc(sizeof(Komponenta));
    if (!nova) {
        perror("Greška alokacije memorije");
        return NULL;
    }

    nova->id = generiraj_novi_id(glava);
    nova->tip = ucitaj_tip_komponente_od_korisnika();

    printf("Unesite brand: ");
    fgets(nova->brand, MAX_BREND, stdin);
    nova->brand[strcspn(nova->brand, "\n")] = 0;

    printf("Unesite model: ");
    fgets(nova->model, MAX_MODEL, stdin);
    nova->model[strcspn(nova->model, "\n")] = 0;

    nova->cijena = ucitaj_float("Unesite cijenu: ");

    nova->kod_kompatibilnosti = ucitaj_int("Unesite kod kompatibilnosti (broj): ");

    nova->sljedeci = NULL;

    return nova;
}

void izmijeni_komponentu(Komponenta* kom) {
    if (!kom) {
        printf("Komponenta nije pronađena.\n");
        return;
    }

    printf("Trenutni brand: %s\n", kom->brand);
    printf("Unesite novi brand (ENTER za preskakanje): ");
    char buffer[MAX_BREND];
    fgets(buffer, MAX_BREND, stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(kom->brand, buffer, MAX_BREND);
    }

    printf("Trenutni model: %s\n", kom->model);
    printf("Unesite novi model (ENTER za preskakanje): ");
    fgets(buffer, MAX_MODEL, stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(kom->model, buffer, MAX_MODEL);
    }

    printf("Trenutna cijena: %.2f\n", kom->cijena);
    printf("Unesite novu cijenu (ENTER za preskakanje): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL && buffer[0] != '\n') {
        kom->cijena = atof(buffer);
    }

    printf("Trenutni kod kompatibilnosti: %d\n", kom->kod_kompatibilnosti);
    printf("Unesite novi kod kompatibilnosti (ENTER za preskakanje): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL && buffer[0] != '\n') {
        kom->kod_kompatibilnosti = atoi(buffer);
    }

    printf("Komponenta uspjesno izmijenjena.\n");
}

void meni_admin(Komponenta** glava) {
    char unos[20];
    int izbor;
    bool autentificiran = false;

    printf("Unesite lozinku: ");
    fgets(unos, sizeof(unos), stdin);
    unos[strcspn(unos, "\n")] = 0;

    if (!provjeri_lozinku_admina(unos)) {
        printf("Neispravna lozinka!\n");
        return;
    }
    autentificiran = true;

    while (autentificiran) {
        printf("\n--- Administratorski meni ---\n");
        printf("1. Dodaj komponentu\n");
        printf("2. Prikazi sve komponente\n");
        printf("3. Izmijeni komponentu\n");
        printf("4. Obrisi komponentu\n");
        printf("5. Spremi komponente u datoteku\n");
        printf("6. Ucitaj komponente iz datoteke\n");
        printf("0. Izlaz\n");
        izbor = ucitaj_int("Odaberite opciju: ");

        switch (izbor) {
        case 1: {
            Komponenta* nova = kreiraj_novu_komponentu(*glava);
            if (nova)
                *glava = dodaj_komponentu(*glava, nova);
            break;
        }
        case 2:
            ispisi_listu_komponenti(*glava);
            break;
        case 3: {
            int id = ucitaj_int("Unesite ID komponente za izmjenu: ");
            Komponenta* kom = pronadi_komponentu_po_id(*glava, id);
            izmijeni_komponentu(kom);
            break;
        }
        case 4: {
            int id = ucitaj_int("Unesite ID komponente za brisanje: ");
            *glava = obrisi_komponentu(*glava, id);
            break;
        }
        case 5:
            if (sacuvaj_komponente_u_datoteku("komponente.bin", *glava))
                printf("Komponente su uspjesno spremljene.\n");
            else
                printf("Greška pri spremanju komponenti.\n");
            break;
        case 6:
            oslobodi_komponente(*glava);
            *glava = ucitaj_komponente_iz_datoteke("komponente.bin");
            if (*glava)
                printf("Komponente su uspješno učitane.\n");
            else
                printf("Greška pri učitavanju komponenti.\n");
            break;
        case 0:
            autentificiran = false;
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    }
}
