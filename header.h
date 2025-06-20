#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_BREND 30
#define MAX_MODEL 50
#define ADMIN_LOZINKA "admin123"
#define BROJ_TIPOVA_KOMPONENTI 7

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef enum {
    CPU = 0,
    GPU,
    RAM,
    STORAGE,
    MATICNA,
    NAPAJANJE,
    KUCISTE
} TipKomponente;

typedef enum {
    IZBORNIK_ADMIN = 1,
    IZBORNIK_KORISNIK,
    IZBORNIK_IZLAZ
} GlavniIzbornik;

typedef union {
    int id_int;
    float id_float;
} KomponentaID;

typedef struct Komponenta {
    int id;
    TipKomponente tip;
    char brand[MAX_BREND];
    char model[MAX_MODEL];
    float cijena;
    int kod_kompatibilnosti;
    struct Komponenta* sljedeci;
} Komponenta;

typedef struct {
    int broj;
    Komponenta** komponente; 
    float ukupna_cijena;
} Konfiguracija;

extern int globalni_brojac;

// Pomoćne funkcije za unos i buffer
void isprazni_buffer(void);
float ucitaj_float(const char* poruka);
int ucitaj_int(const char* poruka);
TipKomponente ucitaj_tip_komponente_od_korisnika(void);
const char* tip_komponente_u_tekst(TipKomponente tip);

// Manipulacija listom komponenti
Komponenta* dodaj_komponentu(Komponenta* glava, Komponenta* nova);
Komponenta* pronadi_komponentu_po_id(Komponenta* glava, int id);
Komponenta* obrisi_komponentu(Komponenta* glava, int id);
void oslobodi_komponente(Komponenta* glava);
void ispisi_komponentu(const Komponenta* kom);
void ispisi_listu_komponenti(const Komponenta* glava);
int generiraj_novi_id(Komponenta* glava);

// Rad sa datotekama
Komponenta* ucitaj_komponente_iz_datoteke(const char* naziv_datoteke);
bool sacuvaj_komponente_u_datoteci(const char* naziv_datoteke, Komponenta* glava);
bool preimenuj_datoteku(const char* stari, const char* novi);
bool obrisi_datoteku(const char* naziv);
bool kopiraj_datoteku(const char* izvor, const char* odrediste);
void prikazi_velicinu_datoteke(const char* naziv);

// Admin funkcije
bool provjeri_lozinku_admina(const char* unos);

// Rad sa konfiguracijom
Konfiguracija* kreiraj_konfiguraciju(void);
void oslobodi_konfiguraciju(Konfiguracija* konf);
bool dodaj_komponentu_u_konfiguraciju(Konfiguracija* konf, Komponenta* kom);
void ispisi_konfiguraciju(const Konfiguracija* konf);

// Sortiranje i pretraga
void sortiraj_komponente(Komponenta** glava, int (*cmp)(const void*, const void*));
Komponenta* pretrazi_komponentu_bsearch(Komponenta** niz, int n, int id);
int cmp_komponente_po_cijeni(const void* a, const void* b);
int cmp_komponente_po_id(const void* a, const void* b);

void provjeri_gresku(FILE* f);

void meni_korisnik(Komponenta* glava);

#endif 
