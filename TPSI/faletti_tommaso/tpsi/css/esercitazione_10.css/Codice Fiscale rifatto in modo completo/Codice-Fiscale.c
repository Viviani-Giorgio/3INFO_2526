#include <stdio.h>      
#include <string.h>    
#include <ctype.h>      

//  PROTOTIPI

void pulisci_parola(char parola[]);
void prendi_3_lettere(char input[], char risultato[], int tipo);
char trova_ultima_lettera(char cf_quasi_finito[]);

int main() {
    //LE VARIABILI
    char nome[50], cognome[50], comune[100], sesso;
    int giorno, mese, anno;
    char codice_fiscale[17] = ""; // Qui dentro costruiremo il codice pezzo dopo pezzo
    char tre_cognome[4], tre_nome[4], cod_comune[5] = "XXXX";

    //CHIEDIAMO I DATI ALL'UTENTE
    printf("SCRIVI IL TUO COGNOME: ");
    scanf("%s", cognome); 
    pulisci_parola(cognome); // Trasforma in maiuscolo e toglie simboli strani

    printf("SCRIVI IL TUO NOME: ");
    scanf("%s", nome);
    pulisci_parola(nome);

    printf("SEI MASCHIO O FEMMINA? (M/F): ");
    scanf(" %c", &sesso); // Lo spazio prima di %c serve a non far saltare la domanda

    printf("IN CHE GIORNO SEI NATO? (es. 15): ");
    scanf("%d", &giorno);

    printf("IN CHE MESE? (es. 3): ");
    scanf("%d", &mese);

    printf("IN CHE ANNO? (es. 1995): ");
    scanf("%d", &anno);

    printf("IN CHE COMUNE SEI NATO? (Tutto attaccato): ");
    scanf("%s", comune);
    pulisci_parola(comune);

    // COSTRUISCO IL CODICE FISCALE

    //Prendiamo le 3 lettere del COGNOME
    prendi_3_lettere(cognome, tre_cognome, 0); // 0 significa "fai il cognome"
    strcat(codice_fiscale, tre_cognome); // strcat significa "attacca alla fine"

    //Prendiamo le 3 lettere del NOME
    prendi_3_lettere(nome, tre_nome, 1); // 1 significa "fai il nome" (regola speciale)
    strcat(codice_fiscale, tre_nome);

    //Aggiungiamo l'ANNO (solo le ultime due cifre)
    int ultime_due_anno = anno % 100; // Es: 1998 diviso 100 dà resto 98
    sprintf(codice_fiscale + 6, "%02d", ultime_due_anno); // Scrive il numero nella stringa

    //Aggiungiamo il MESE (trasformato in lettera)
    char tabella_mesi[] = "ABCDEHLMPRST"; // Gennaio=A, Febbraio=B, ecc.
    codice_fiscale[8] = tabella_mesi[mese - 1]; // -1 perché gli array partono da zero

    //Aggiungiamo il GIORNO e il SESSO
    if (sesso == 'F' || sesso == 'f') {
        giorno = giorno + 40; // Se è femmina aggiungiamo 40 come dice il PDF
    }
    sprintf(codice_fiscale + 9, "%02d", giorno); // Lo scrive nelle posizioni 9 e 10

    //Cerchiamo il CODICE COMUNE nel file comuni_cod.txt
    FILE *file_comuni;
    file_comuni = fopen("comuni_cod.txt", "r"); // Apre il file in lettura
    if (file_comuni != NULL) {
        char riga[200];
        while (fgets(riga, 200, file_comuni)) { // Legge il file riga per riga
            if (strstr(riga, comune)) { // Se in questa riga c'è scritto il nostro comune
                strncpy(cod_comune, riga, 4); // Prende i primi 4 caratteri 
                cod_comune[4] = '\0'; 
                break; // Smette di cercare
            }
        }
        fclose(file_comuni); // Chiude il file per non sprecare memoria
    }
    strcat(codice_fiscale, cod_comune); // Attacca il codice del comune

    // (Controllo)
    codice_fiscale[15] = trova_ultima_lettera(codice_fiscale);
    codice_fiscale[16] = '\0';

    // RISULTATO FINALE
    printf("\n----------------------------------\n");
    printf("IL TUO CODICE FISCALE E': %s\n", codice_fiscale);
    printf("----------------------------------\n");

    return 0; 
}

//FUNZIONI 

// Questa funzione pulisce la parola e la rende tutta grande
void pulisci_parola(char parola[]) {
    for (int i = 0; i < strlen(parola); i++) {
        parola[i] = toupper(parola[i]); // toupper trasforma 'a' in 'A'
    }
}

// Questa funzione estrae le 3 lettere 
void prendi_3_lettere(char input[], char risultato[], int tipo) {
    char cons[50] = "", voc[50] = "";
    int c = 0, v = 0;

    // Divido consonanti e vocali
    for (int i = 0; i < strlen(input); i++) {
        char lettera = input[i];
        if (lettera == 'A' || lettera == 'E' || lettera == 'I' || lettera == 'O' || lettera == 'U') {
            voc[v++] = lettera;
        } else if (lettera >= 'A' && lettera <= 'Z') {
            cons[c++] = lettera;
        }
    }

    // Se stiamo facendo il NOME e ci sono almeno 4 consonanti (Regola speciale)
    if (tipo == 1 && c >= 4) {
        risultato[0] = cons[0]; // Prima
        risultato[1] = cons[2]; // Terza
        risultato[2] = cons[3]; // Quarta
    } else {
        // Regola normale per cognome e nomi corti
        int riempiti = 0;
        int j = 0;
        while (riempiti < 3 && j < c) risultato[riempiti++] = cons[j++]; // Usa consonanti
        j = 0;
        while (riempiti < 3 && j < v) risultato[riempiti++] = voc[j++];  // Se mancano, usa vocali
        while (riempiti < 3) risultato[riempiti++] = 'X';              // Se mancano, usa X
    }
    risultato[3] = '\0'; // Fine della stringa di 3 lettere
}

// Questa funzione calcola l'ultima lettera
char trova_ultima_lettera(char cf[]) {
    // Tabella per le posizioni dispari (1a, 3a, 5a... lettera del codice)
    int tab_dispari[] = {1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 2, 4, 18, 20, 11, 3, 6, 8, 12, 14, 16, 10, 22, 25, 24, 23};
    int somma = 0;

    for (int i = 0; i < 15; i++) {
        int valore;
        if (cf[i] >= '0' && cf[i] <= '9') valore = cf[i] - '0'; 
        else valore = cf[i] - 'A'; 

        if ((i + 1) % 2 != 0) { // Se è in posizione dispari
            somma = somma + tab_dispari[valore];
        } else { // Se è in posizione pari
            somma = somma + valore;
        }
    }
    return (somma % 26) + 'A'; // Il resto della somma diviso 26 dà la lettera finale
}