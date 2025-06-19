#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>

#define MAX_BREND 30
#define MAX_MODEL 50
#define ADMIN_LOZINKA "admin123"
#define BROJ_TIPOVA_KOMPONENTI 7

typedef enum {
    CPU = 0,
    GPU,
    RAM,
    STORAGE,
    MATICNA,
    NAPAJANJE,
    KUCISTE
} TipKomponente;

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
    Komponenta** komponente;  // niz pokazivaca na komponente, jedan po tipu
    float ukupna_cijena;
} Konfiguracija;

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
bool sacuvaj_komponente_u_datoteku(const char* naziv_datoteke, Komponenta* glava);

// Admin funkcije
bool provjeri_lozinku_admina(const char* unos);

// Rad sa konfiguracijom
Konfiguracija* kreiraj_konfiguraciju(void);
void oslobodi_konfiguraciju(Konfiguracija* konf);
bool dodaj_komponentu_u_konfiguraciju(Konfiguracija* konf, Komponenta* kom);
void ispisi_konfiguraciju(const Konfiguracija* konf);

#endif // HEADER_H
