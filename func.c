#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void isprazni_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float ucitaj_float(const char* poruka) {
    float broj;
    printf("%s", poruka);
    while (scanf("%f", &broj) != 1) {
        printf("Neispravan unos, pokusajte ponovno: ");
        isprazni_buffer();
    }
    isprazni_buffer();
    return broj;
}

int ucitaj_int(const char* poruka) {
    int broj;
    printf("%s", poruka);
    while (scanf("%d", &broj) != 1) {
        printf("Neispravan unos, pokusajte ponovno: ");
        isprazni_buffer();
    }
    isprazni_buffer();
    return broj;
}

TipKomponente ucitaj_tip_komponente_od_korisnika() {
    int izbor;
    do {
        printf("Odaberite tip komponente:\n");
        printf("0 - CPU\n1 - GPU\n2 - RAM\n3 - MEMORIJA\n4 - MATICNA\n5 - NAPAJANJE\n6 - KUCISTE\n");
        izbor = ucitaj_int("Unesite broj: ");
    } while (izbor < 0 || izbor >= BROJ_TIPOVA_KOMPONENTI);
    return (TipKomponente)izbor;
}

const char* tip_komponente_u_tekst(TipKomponente tip) {
    static const char* tipovi[] = { "CPU", "GPU", "RAM", "MEMORIJA", "MATICNA", "NAPAJANJE", "KUCISTE" };
    if (tip < 0 || tip >= BROJ_TIPOVA_KOMPONENTI) return "Nepoznat";
    return tipovi[tip];
}

Komponenta* dodaj_komponentu(Komponenta* glava, Komponenta* nova) {
    if (!glava) return nova;

    Komponenta* temp = glava;
    while (temp->sljedeci)
        temp = temp->sljedeci;
    temp->sljedeci = nova;
    return glava;
}

Komponenta* pronadi_komponentu_po_id(Komponenta* glava, int id) {
    while (glava) {
        if (glava->id == id)
            return glava;
        glava = glava->sljedeci;
    }
    return NULL;
}

Komponenta* obrisi_komponentu(Komponenta* glava, int id) {
    Komponenta* trenutni = glava;
    Komponenta* prethodni = NULL;

    while (trenutni) {
        if (trenutni->id == id) {
            if (prethodni)
                prethodni->sljedeci = trenutni->sljedeci;
            else
                glava = trenutni->sljedeci;
            free(trenutni);
            printf("Komponenta obrisana.\n");
            return glava;
        }
        prethodni = trenutni;
        trenutni = trenutni->sljedeci;
    }
    printf("Komponenta s tim ID-om nije pronađena.\n");
    return glava;
}

void oslobodi_komponente(Komponenta* glava) {
    while (glava) {
        Komponenta* sljedeci = glava->sljedeci;
        free(glava);
        glava = sljedeci;
    }
}

void ispisi_komponentu(const Komponenta* kom) {
    if (!kom) return;
    printf("ID: %d | Tip: %s | Brand: %s | Model: %s | Cijena: %.2f | Kod kompatibilnosti: %d\n",
        kom->id, tip_komponente_u_tekst(kom->tip), kom->brand, kom->model, kom->cijena, kom->kod_kompatibilnosti);
}

void ispisi_listu_komponenti(const Komponenta* glava) {
    if (!glava) {
        printf("Lista komponenti je prazna.\n");
        return;
    }
    while (glava) {
        ispisi_komponentu(glava);
        glava = glava->sljedeci;
    }
}

int generiraj_novi_id(Komponenta* glava) {
    int max_id = 0;
    while (glava) {
        if (glava->id > max_id)
            max_id = glava->id;
        glava = glava->sljedeci;
    }
    return max_id + 1;
}

Komponenta* ucitaj_komponente_iz_datoteke(const char* naziv_datoteke) {
    FILE* dat = fopen(naziv_datoteke, "rb");
    if (!dat) return NULL;

    Komponenta* glava = NULL;
    Komponenta* zadnji = NULL;
    Komponenta temp;

    while (fread(&temp, sizeof(Komponenta), 1, dat) == 1) {
        Komponenta* novi = (Komponenta*)malloc(sizeof(Komponenta));
        if (!novi) {
            perror("Greska pri alokaciji memorije");
            oslobodi_komponente(glava);
            fclose(dat);
            return NULL;
        }
        *novi = temp;
        novi->sljedeci = NULL;

        if (!glava)
            glava = novi;
        else
            zadnji->sljedeci = novi;
        zadnji = novi;
    }

    fclose(dat);
    return glava;
}

bool sacuvaj_komponente_u_datoteku(const char* naziv_datoteke, Komponenta* glava) {
    FILE* dat = fopen(naziv_datoteke, "wb");
    if (!dat) {
        perror("Greska pri otvaranju datoteke za zapis");
        return false;
    }

    while (glava) {
        if (fwrite(glava, sizeof(Komponenta), 1, dat) != 1) {
            perror("Greska pri pisanju u datoteku");
            fclose(dat);
            return false;
        }
        glava = glava->sljedeci;
    }

    fclose(dat);
    return true;
}

Konfiguracija* kreiraj_konfiguraciju() {
    Konfiguracija* konf = (Konfiguracija*)calloc(1, sizeof(Konfiguracija));
    if (!konf) return NULL;
    konf->broj = 0;
    konf->ukupna_cijena = 0.0f;
    konf->komponente = (Komponenta**)calloc(BROJ_TIPOVA_KOMPONENTI, sizeof(Komponenta*));
    if (!konf->komponente) {
        free(konf);
        return NULL;
    }
    return konf;
}

void oslobodi_konfiguraciju(Konfiguracija* konf) {
    if (!konf) return;
    free(konf->komponente);
    free(konf);
}

bool dodaj_komponentu_u_konfiguraciju(Konfiguracija* konf, Komponenta* kom) {
    if (!konf || !kom) return false;
    if (konf->komponente[kom->tip] != NULL) {
        printf("Tip komponente %s je već dodan u konfiguraciju.\n", tip_komponente_u_tekst(kom->tip));
        return false;
    }
    konf->komponente[kom->tip] = kom;
    konf->broj++;
    konf->ukupna_cijena += kom->cijena;
    return true;
}

void ispisi_konfiguraciju(const Konfiguracija* konf) {
    if (!konf) {
        printf("Konfiguracija je prazna.\n");
        return;
    }
    printf("\n--- Konfiguracija ---\n");
    for (int i = 0; i < BROJ_TIPOVA_KOMPONENTI; i++) {
        if (konf->komponente[i]) {
            ispisi_komponentu(konf->komponente[i]);
        }
    }
    printf("Ukupna cijena: %.2f\n", konf->ukupna_cijena);
}

void pretrazi_komponente(Komponenta* glava) {
    if (!glava) {
        printf("Nema dostupnih komponenti.\n");
        return;
    }

    TipKomponente trazeniTip = ucitaj_tip_komponente_od_korisnika();

    char trazeniBrand[MAX_BREND];
    printf("Unesite brend za pretragu (ENTER za preskakanje): ");
    fgets(trazeniBrand, MAX_BREND, stdin);
    trazeniBrand[strcspn(trazeniBrand, "\n")] = 0;

    float maxCijena = -1;
    printf("Unesite maksimalnu cijenu (0 za preskakanje): ");
    scanf("%f", &maxCijena);
    isprazni_buffer();

    int count = 0;
    for (Komponenta* tek = glava; tek != NULL; tek = tek->sljedeci) {
        if (tek->tip == trazeniTip) {
            bool brandUredu = (strlen(trazeniBrand) == 0) || (_stricmp(tek->brand, trazeniBrand) == 0);
            bool cijenaUredu = (maxCijena <= 0) || (tek->cijena <= maxCijena);
            if (brandUredu && cijenaUredu) {
                ispisi_komponentu(tek);
                count++;
            }
        }
    }

    if (count == 0) {
        printf("Nema komponenti koje zadovoljavaju kriterije.\n");
    }
}

void korisnicko_slaganje(Komponenta* glava) {
    if (!glava) {
        printf("Nema dostupnih komponenti.\n");
        return;
    }

    Konfiguracija* konf = kreiraj_konfiguraciju();
    if (!konf) {
        printf("Neuspjelo kreiranje konfiguracije.\n");
        return;
    }

    int izbor = 0;
    while (true) {
        printf("\n--- Korisnicko slaganje ---\n");
        printf("1 - Dodaj komponentu u konfiguraciju\n");
        printf("2 - Prikazi trenutnu konfiguraciju\n");
        printf("3 - Izbrisi komponentu iz konfiguracije\n");
        printf("4 - Završetak slaganja\n");
        izbor = ucitaj_int("Unesite izbor: ");

        if (izbor == 1) {
            ispisi_listu_komponenti(glava);
            int id = ucitaj_int("Unesite ID komponente za dodavanje: ");
            Komponenta* kom = pronadi_komponentu_po_id(glava, id);
            if (kom) {
                if (dodaj_komponentu_u_konfiguraciju(konf, kom))
                    printf("Komponenta dodana.\n");
            }
            else {
                printf("Komponenta s tim ID-om nije pronađena.\n");
            }
        }
        else if (izbor == 2) {
            ispisi_konfiguraciju(konf);
        }
        else if (izbor == 3) {
            int tip = ucitaj_int("Unesite tip komponente koju želite izbrisati iz konfiguracije (0-6): ");
            if (tip < 0 || tip >= BROJ_TIPOVA_KOMPONENTI) {
                printf("Neispravan tip.\n");
                continue;
            }
            if (konf->komponente[tip]) {
                konf->ukupna_cijena -= konf->komponente[tip]->cijena;
                konf->komponente[tip] = NULL;
                konf->broj--;
                printf("Komponenta izbrisana iz konfiguracije.\n");
            }
            else {
                printf("Ta komponenta nije u konfiguraciji.\n");
            }
        }
        else if (izbor == 4) {
            break;
        }
        else {
            printf("Neispravan unos.\n");
        }
    }

    oslobodi_konfiguraciju(konf);
}

void nasumicna_konfiguracija_u_budzetu(Komponenta* glava, float budzet) {
    if (!glava) {
        printf("Nema dostupnih komponenti.\n");
        return;
    }
    if (budzet <= 0) {
        printf("Budzet mora biti veci od nule.\n");
        return;
    }

    srand((unsigned int)time(NULL));

    Konfiguracija* konf = kreiraj_konfiguraciju();
    if (!konf) {
        printf("Neuspjelo kreiranje konfiguracije.\n");
        return;
    }

    float budzetPoTipu = budzet / BROJ_TIPOVA_KOMPONENTI;

    for (int tip = 0; tip < BROJ_TIPOVA_KOMPONENTI; tip++) {

        Komponenta* kandidati[1000]; 
        int kandidatBroj = 0;

        for (Komponenta* tek = glava; tek != NULL; tek = tek->sljedeci) {
            if (tek->tip == tip && tek->cijena <= budzetPoTipu) {
                kandidati[kandidatBroj++] = tek;
            }
        }

        if (kandidatBroj == 0) {

            for (Komponenta* tek = glava; tek != NULL; tek = tek->sljedeci) {
                if (tek->tip == tip) {
                    kandidati[kandidatBroj++] = tek;
                }
            }
        }

        if (kandidatBroj > 0) {

            int index = rand() % kandidatBroj;
            if (dodaj_komponentu_u_konfiguraciju(konf, kandidati[index])) {

                budzet -= kandidati[index]->cijena;
                if (budzet < 0) budzet = 0;

                int preostalo = BROJ_TIPOVA_KOMPONENTI - tip - 1;
                if (preostalo > 0)
                    budzetPoTipu = budzet / preostalo;
            }
        }
        else {
            printf("Nema dostupnih komponenti za tip %s\n", tip_komponente_u_tekst(tip));
        }
    }

    ispisi_konfiguraciju(konf);

    oslobodi_konfiguraciju(konf);
}

void meni_korisnik(Komponenta* glava) {
    int izbor = 0;
    while (true) {
        printf("\n--- Korisnicki meni ---\n");
        printf("1 - Pregled svih komponenti\n");
        printf("2 - Pretraga komponenti\n");
        printf("3 - Slaganje konfiguracije\n");
        printf("5 - Nasumicna konfiguracija u budzetu\n");
        printf("6 - Izlaz\n");
        izbor = ucitaj_int("Unesite izbor: ");

        switch (izbor) {
        case 1:
            ispisi_listu_komponenti(glava);
            break;
        case 2:
            pretrazi_komponente(glava);
            break;
        case 3:
            korisnicko_slaganje(glava);
            break;
        case 4:
            printf("Izlaz iz korisnickog menija.\n");
            return;
        case 5: {
            float budzet = ucitaj_float("Unesite maksimalni budzet za konfiguraciju: ");
            nasumicna_konfiguracija_u_budzetu(glava, budzet);
            break;
        }
        case 6: {
            return;
        }
        default:
            printf("Nepoznat izbor, pokusajte ponovno.\n");
        }
    }
}
