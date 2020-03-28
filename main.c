/* LUCA Alexandra - 321CC */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

#define DEBUG 1


// cauta tabelul <nume_tabel> in baza de date
// returneaza pointer la tabel daca exista, si NULL daca nu exista
t_table* findTable(t_db baza, char *nume_tabel) 
{

}

// dezaloca linia int
void freeIntLine(t_intLine *linie) {

}

// dezaloca linia float
void freeFloatLine(t_floatLine *linie) {

}

// dezaloca linia string
void freeStringLine(t_stringLine *linie) {

}

// dezaloca linia
void freeLine(void *linie, t_cellType tip) {
	if (tip == INT)
		freeIntLine(linie);
}

// dezaloca toate liniile din tabel
void freeLinesInTable(t_table *tp) {
	//freeLine(tp->lines, tp->type);
}

// dezaloca tabelul cu totul
void freeTable(t_table *tp) {

}

// afiseaza header de tabel
void printTableHeader(char *nume_tabel, t_column *lista_de_coloane) {

}

// afiseaza linia int
void printIntLine(t_intLine *linie) {

}

// afiseaza linia float
void printFloatLine(t_floatLine *linie) {

}

// afiseaza linia string
void printStringLine(t_stringLine *linie) {

}

// afiseaza linia
void printLine(void *linie, t_cellType tip) {

}

// afiseaza tabel
void printTable(t_table *tp) {

}

// afiseaza baza de date
void printDatabase(t_db baza) {
	printf("DATABASE: %s\n\n", baza.name);

	// afisez fiecare tabel
}


int main()
{
	char linie[MAX_CMD_LEN];
	char comanda[MAX_CMD_LEN];
	char nume_tabel[MAX_CMD_LEN];
	char tip_de_date[MAX_CMD_LEN];
	char *p;

	t_table *tp;
	t_table *tabel_nou;

	t_column *tcol;
	t_column *coloana_noua;

	t_db baza_de_date;

	while (1) {
		// citesc o linie
		fgets(linie, MAX_CMD_LEN, stdin);

		// elimin newline de la final pus de fgets
		if (linie[strlen(linie)-1] == '\n') {
			linie[strlen(linie)-1] = '\0';
		}
		DEBUG && printf("Am citit: '%s'\n", linie);

		// gasesc primul cuvant din linie
		p = strtok(linie, " ");
		strcpy(comanda, p);

		if (strcmp(comanda, "INIT_DB") == 0) {
			DEBUG && printf("Am primit comanda INIT_DB\n");

			// comanda: INIT_DB <nume>
			// gasesc numele bazei de date
			p = strtok(NULL, " ");

			// initializez baza de date
			strcpy(baza_de_date.name, p);
			baza_de_date.tables = NULL;

		} else if (strcmp(comanda, "DELETE_DB") == 0) {
			DEBUG && printf("Am primit comanda DELETE_DB\n");

			// sterg memoria alocata

			break;
		} else if (strcmp(comanda, "PRINT_DB") == 0) {
			DEBUG && printf("Am primit comanda PRINT_DB\n");

			printDatabase(baza_de_date);

		} else if (strcmp(comanda, "CREATE") == 0) {
			DEBUG && printf("Am primit comanda CREATE\n");

			// comanda: CREATE <nume tabel> <tip de date> <coloana1> <coloana2> ...

			// citesc al doilea cuvant: <nume tabel>
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			// citesc al treilea cuvant: <tip de date>
			p = strtok(NULL, " ");
			strcpy(tip_de_date, p);

			if (baza_de_date.tables == NULL) {
				// asta e primul tabel adaugat

				// creez tabel
				baza_de_date.tables = (t_table*)malloc(sizeof(t_table));
				// verificare malloc
				if (baza_de_date.tables == NULL) {
					return -1;
				}
				
				// tabel_nou este tabelul nou creat
				tabel_nou = baza_de_date.tables;
			} else {
				// asta nu e primul tabel

				// parcurg pana la ultimul tabel
				tp = baza_de_date.tables;
				while (tp->next != NULL) {
					tp = tp->next;
				}

				// creez tabel
				tp->next = (t_table*)malloc(sizeof(t_table));
				// verificare malloc
				if (tp->next == NULL) {
					return -1;
				}

				// tabel_nou este tabelul nou creat
				tabel_nou = tp->next;
			}
			
			// configurez campurile din tabel
			tabel_nou->next = NULL;
			strcpy(tabel_nou->name, nume_tabel);
			if (strcmp(tip_de_date, "INT") == 0) {
				tabel_nou->type = INT;
			} else if (strcmp(tip_de_date, "FLOAT") == 0) {
				tabel_nou->type = FLOAT;
			} else if (strcmp(tip_de_date, "STRING") == 0) {
				tabel_nou->type = STRING;
			} else {
				printf("Unknown data type: \"%s\"\n", tip_de_date);
			}
			tabel_nou->lines = NULL;
			tabel_nou->columns = NULL;

			// configurez coloanele din tabel
			p = strtok(NULL, " ");
			while (p != NULL) {
				DEBUG && printf("Creez coloana '%s'\n", p);

				coloana_noua = (t_column*)malloc(sizeof(t_column));
				if (coloana_noua == NULL) {
					return -1;
				}
				strcpy(coloana_noua->name, p);
				coloana_noua->next = NULL;

				if (tabel_nou->columns == NULL) {
					tabel_nou->columns = coloana_noua;
					tcol = tabel_nou->columns;
				} else {
					// tcol pointeaza mereu catre ultima coloana existenta
					tcol->next = coloana_noua;
					tcol = tcol->next;
				}

				p = strtok(NULL, " ");
			}

		} else {
			printf("Unknown command: \"%s\"\n", comanda);
		}
	}

	return 0;
}