/* LUCA Alexandra - 321CC */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

#define DEBUG 1


// cauta tabelul <nume_tabel> in baza de date
// returneaza pointer la tabel daca exista, si NULL daca nu exista
t_table* findTable(t_db *baza, char *nume_tabel) 
{
	t_table *p;
	p = baza->tables;

	while (p != NULL) {
		if (strcmp(p->name, nume_tabel) == 0) {
			return p;
		}

		p = p->next;
	}

	return NULL;
}

// dezaloca celulele int
void freeIntCellsInLine(t_intLine *linie) {
	t_intCell *p, *q;
	p = linie->cells;

	while (p != NULL) {
		q = p;
		p = p->next;

		free(q);
	}

	linie->cells = NULL;
}

// dezaloca celulele float
void freeFloatCellsInLine(t_floatLine *linie) {
	t_floatCell *p, *q;
	p = linie->cells;

	while (p != NULL) {
		q = p;
		p = p->next;

		free(q);
	}

	linie->cells = NULL;
}

// dezaloca celulele string
void freeStringCellsInLine(t_stringLine *linie) {
	t_stringCell *p, *q;
	p = linie->cells; 

	while (p != NULL) {
		q = p;
		p = p->next;

		free(q->value);
		free(q);
	}

	linie->cells = NULL;
}

// dezaloca toate celulele din linie
void freeCellsInLine(void *linie, t_cellType tip) {
	if (tip == INT)
		freeIntCellsInLine((t_intLine*)linie);
	else if (tip == FLOAT)
		freeFloatCellsInLine((t_floatLine*)linie);
	else if (tip == STRING)
		freeStringCellsInLine((t_stringLine*)linie);
}

// dezaloca toate liniile din tabel
void freeLinesInTable(t_table *tp) {
	void *p, *q;
	p = tp->lines;

	while (p != NULL) {
		q = p;

		if (tp->type == INT)
			p = ((t_intLine*)p)->next;
		else if (tp->type == FLOAT)
			p = ((t_floatLine*)p)->next;
		else if (tp->type == STRING)
			p = ((t_stringLine*)p)->next;

		freeCellsInLine(q, tp->type);
		free(q);
	}

	tp->lines = NULL;
}

// dezaloca toate coloanele din tabel
void freeColumnsInTable(t_table *tp) {
	t_column *p, *q;
	p = tp->columns;

	while (p != NULL) {
		q = p;
		p = p->next;

		free(q);
	}

	tp->columns = NULL;
}

// dezaloca tabelul cu totul
void freeTable(t_table *tp) {
	freeColumnsInTable(tp);
	freeLinesInTable(tp);
}

// afiseaza header de tabel
void printTableHeader(char *nume_tabel, t_column *lista_de_coloane) {
	printf("TABLE: %s\n", nume_tabel);

	t_column *p;
	p = lista_de_coloane;

	while (p != NULL) {
		printf("%-*s ", MAX_COLUMN_NAME_LEN, p->name);

		p = p->next;
	}

	printf("\n");
	p = lista_de_coloane;

	while (p != NULL) {
		int i;
		for (i=1; i<=MAX_COLUMN_NAME_LEN; i++)
			printf("-");
		printf(" ");

		p = p->next;
	}

	printf("\n");
}

// afiseaza linia int
void printIntLine(t_intLine *linie) {
	t_intCell *p;
	p = linie->cells;

	while (p != NULL) {
		printf("%d", p->value);
		p = p->next;
	}
}

// afiseaza linia float
void printFloatLine(t_floatLine *linie) {
	t_floatCell *p;
	p = linie->cells;

	while (p != NULL) {
		printf("%f", p->value);
		p = p->next;
	}
}

// afiseaza linia string
void printStringLine(t_stringLine *linie) {
	t_stringCell *p;
	p = linie->cells;

	while (p != NULL) {
		printf("%s", p->value);
		p = p->next;
	}
}

// afiseaza linia
void printLine(void *linie, t_cellType tip) {
	if (tip == INT)
		printIntLine((t_intLine*)linie);
	else if (tip == FLOAT)
		printFloatLine((t_floatLine*)linie);
	else if (tip == STRING)
		printStringLine((t_stringLine*)linie);
}

// afiseaza tabel
void printTable(t_table *tp) {
	printTableHeader(tp->name, tp->columns);

	void *p;
	p = tp->lines;

	while (p != NULL) {
		printLine(p, tp->type);

		if (tp->type == INT)
			p = ((t_intLine*)p)->next;
		else if (tp->type == FLOAT)
			p = ((t_floatLine*)p)->next;
		else if (tp->type == STRING)
			p = ((t_stringLine*)p)->next;
	}

	printf("\n");
}

// afiseaza baza de date
void printDatabase(t_db *baza) {
	printf("DATABASE: %s\n\n", baza->name);

	t_table *p;
	p = baza->tables;

	while (p != NULL) {
		printTable(p);
		p = p->next;
	}
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

			printDatabase(&baza_de_date);

		} else if (strcmp(comanda, "PRINT") == 0) {
			DEBUG && printf("Am primit comanda PRINT\n");

			// comanda: PRINT <nume_tabel>
			// gasesc numele tabelului
			p = strtok(NULL, " ");

			// gasesc tabelul in baza de date
			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database\n", p);
			} else {
				printTable(t);
			}

		} else if (strcmp(comanda, "CLEAR") == 0) {
			DEBUG && printf("Am primit comanda CLEAR\n");

			// comanda: CLEAR <nume_tabel>
			// gasesc numele tabelului
			p = strtok(NULL, " ");

			// gasesc tabelul in baza de date
			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database\n", p);
			} else {
				freeLinesInTable(t);
			}

		} else if (strcmp(comanda, "CREATE") == 0) {
			DEBUG && printf("Am primit comanda CREATE\n");

			// comanda: CREATE <nume tabel> <tip de date> <coloana1> <coloana2> ...

			// citesc al doilea cuvant: <nume tabel>
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			t_table *t = findTable(&baza_de_date, p);
			if (t != NULL) {
				printf("Table \"%s\" already exists\n", p);
				continue;
			}

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