/* LUCA Alexandra - 321CC */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

#define DEBUG 0


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
		printf("%-*d ", MAX_COLUMN_NAME_LEN, p->value);
		p = p->next;
	}
	printf("\n");
}

// afiseaza linia float
void printFloatLine(t_floatLine *linie) {
	t_floatCell *p;
	p = linie->cells;

	while (p != NULL) {
		printf("%-*f ", MAX_COLUMN_NAME_LEN, p->value);
		p = p->next;
	}
	printf("\n");
}

// afiseaza linia string
void printStringLine(t_stringLine *linie) {
	t_stringCell *p;
	p = linie->cells;

	while (p != NULL) {
		printf("%-*s ", MAX_COLUMN_NAME_LEN, p->value);
		p = p->next;
	}
	printf("\n");
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

// returneaza a cata coloana este cea cu numele col_name, sau -1 daca nu exista
int getIndexOfColumn(t_table *tp, char *col_name) {
	t_column *cp;
	cp = tp->columns;

	int index = 0;
	while (cp != NULL) {
		if (strcmp(cp->name, col_name) == 0) {
			return index;
		}

		cp = cp->next;
		index++;
	}

	return -1;
}

// returneaza celula corespunzatoare coloanei cu numele col_name din linie 
// sau NULL daca coloana nu exista 
void* getCellInColumn(void *lp, t_table *tp, char *col_name) {
	int index = getIndexOfColumn(tp, col_name);

	if (index == -1) {
		return NULL;
	}

	if (tp->type == INT) {
		t_intLine *p = (t_intLine*)lp;
		t_intCell *q = p->cells;
		int i;
		for (i=0; i<index; i++) {
			q = q->next;
		}
		return (void*)q;
	} else if (tp->type == FLOAT) {
		t_floatLine *p = (t_floatLine*)lp;
		t_floatCell *q = p->cells;
		int i;
		for (i=0; i<index; i++) {
			q = q->next;
		}
		return (void*)q;
	} else if (tp->type == STRING) {
		t_stringLine *p = (t_stringLine*)lp;
		t_stringCell *q = p->cells;
		int i;
		for (i=0; i<index; i++) {
			q = q->next;
		}
		return (void*)q;
	}
}

// returneaza 1 daca celula indeplineste relatia, si 0 altfel
int checkRelation(void* cell, t_cellType type, char *rel, char *val) {
	if (type == INT) {
		t_intCell *c = (t_intCell*)cell;
		int x = c->value;
		int v = atoi(val);

		if (strcmp(rel, "<") == 0)
			return x < v;
		if (strcmp(rel, "<=") == 0)
			return x <= v;
		if (strcmp(rel, "==") == 0)
			return x == v;
		if (strcmp(rel, "!=") == 0)
			return x != v;
		if (strcmp(rel, ">=") == 0)
			return x >= v;
		if (strcmp(rel, ">") == 0)
			return x > v;
	} else if (type == FLOAT) {
		t_floatCell *c = (t_floatCell*)cell;
		float x = c->value;
		float v = atof(val);

		if (strcmp(rel, "<") == 0)
			return x < v;
		if (strcmp(rel, "<=") == 0)
			return x <= v;
		if (strcmp(rel, "==") == 0)
			return x == v;
		if (strcmp(rel, "!=") == 0)
			return x != v;
		if (strcmp(rel, ">=") == 0)
			return x >= v;
		if (strcmp(rel, ">") == 0)
			return x > v;
	} else if (type == STRING) {
		t_stringCell *c = (t_stringCell*)cell;
		char *x = c->value;
		char *v = val;

		if (strcmp(rel, "<") == 0)
			return strcmp(x, v) < 0;
		if (strcmp(rel, "<=") == 0)
			return strcmp(x, v) <= 0;
		if (strcmp(rel, "==") == 0)
			return strcmp(x, v) == 0;
		if (strcmp(rel, "!=") == 0)
			return strcmp(x, v) != 0;
		if (strcmp(rel, ">=") == 0)
			return strcmp(x, v) >= 0;
		if (strcmp(rel, ">") == 0)
			return strcmp(x, v) > 0;
	}
}


int main()
{
	char linie[MAX_CMD_LEN];
	char comanda[MAX_CMD_LEN];
	char nume_tabel[MAX_CMD_LEN];
	char tip_de_date[MAX_CMD_LEN];
	char nume_coloana[MAX_CMD_LEN];
	char relatie[MAX_CMD_LEN];
	char valoare[MAX_CMD_LEN];
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
			t_table *tp = baza_de_date.tables;
			t_table *tabel_urmator;

			while (tp != NULL) {
				tabel_urmator = tp->next;

				freeTable(tp);
				free(tp);

				tp = tabel_urmator;
			}

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
				printf("Table \"%s\" not found in database.\n", p);
				continue;
			} else {
				printTable(t);
			}

		} else if (strcmp(comanda, "SEARCH") == 0) {
			DEBUG && printf("Am primit comanda SEARCH\n");

			// comanda: SEARCH <nume_tabel> <nume_coloana> <relatie> <valoare>
			// gasesc numele tabelului
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			// gasesc tabelul in baza de date
			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database.\n", p);
				continue;
			} 

			// gasesc numele coloanei
			p = strtok(NULL, " ");
			strcpy(nume_coloana, p);
			// gasesc relatia
			p = strtok(NULL, " ");
			strcpy(relatie, p);
			// gasesc valoarea
			p = strtok(NULL, " ");
			strcpy(valoare, p);
			
			int index = getIndexOfColumn(t, nume_coloana);
			if (index == -1) {
				printf("Table \"%s\" does not contain column \"%s\".\n", nume_tabel, nume_coloana);
				continue;
			}

			printTableHeader(t->name, t->columns);

			void *lp = t->lines;
			while (lp != NULL) {
				void *cell = getCellInColumn(lp, t, nume_coloana);
				if (checkRelation(cell, t->type, relatie, valoare)) {
					printLine(lp, t->type);
				}

				if (t->type == INT)
					lp = ((t_intLine*)lp)->next;
				else if (t->type == FLOAT)
					lp = ((t_floatLine*)lp)->next;
				else if (t->type == STRING)
					lp = ((t_stringLine*)lp)->next;
			}

			printf("\n");

		} else if (strcmp(comanda, "DELETE") == 0) {
			DEBUG && printf("Am primit comanda DELETE\n");

			// comanda: DELETE <nume_tabel> <nume_coloana> <relatie> <valoare>
			// gasesc numele tabelului
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			// gasesc tabelul in baza de date
			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database.\n", p);
				continue;
			} 

			// gasesc numele coloanei
			p = strtok(NULL, " ");

			if (p == NULL) {
				// comanda: DELETE <nume_tabel>
				t_table *tp = baza_de_date.tables;
				t_table *tabel_anterior = tp;
				t_table *tabel_urmator;

				while (tp != NULL) {
					if (strcmp(tp->name, nume_tabel) == 0) {
						freeTable(tp);

						tabel_urmator = tp->next;

						if (tabel_anterior == tp) {
							// asta este primul tabel
							baza_de_date.tables = tabel_urmator;
							free(tp);
							tp = tabel_urmator;
							continue;
						} else {
							// asta nu este primul tabel
							tabel_anterior->next = tabel_urmator;
							free(tp);
							tp = tabel_urmator;
							continue;
						}
					}

					tabel_anterior = tp;
					tp = tp->next;
				}

				continue;
			}

			strcpy(nume_coloana, p);
			// gasesc relatia
			p = strtok(NULL, " ");
			strcpy(relatie, p);
			// gasesc valoarea
			p = strtok(NULL, " ");
			strcpy(valoare, p);
			
			int index = getIndexOfColumn(t, nume_coloana);
			if (index == -1) {
				printf("Table \"%s\" does not contain column \"%s\".\n", nume_tabel, nume_coloana);
				continue;
			}

			void *lp = t->lines;
			void *linia_anterioara = lp;
			void *linia_urmatoare;
			while (lp != NULL) {
				void *cell = getCellInColumn(lp, t, nume_coloana);
				if (checkRelation(cell, t->type, relatie, valoare)) {
					freeCellsInLine(lp, t->type);

					if (t->type == INT)
						linia_urmatoare = ((t_intLine*)lp)->next;
					else if (t->type == FLOAT)
						linia_urmatoare = ((t_floatLine*)lp)->next;
					else if (t->type == STRING)
						linia_urmatoare = ((t_stringLine*)lp)->next;

					if (linia_anterioara == lp) {
						// ma aflu pe prima linie
						t->lines = linia_urmatoare;
						free(lp);
						lp = linia_urmatoare;
						continue;
					} else {
						// nu ma aflu pe prima linie deci refac legaturile
						if (t->type == INT)
							((t_intLine*)linia_anterioara)->next = linia_urmatoare;
						else if (t->type == FLOAT)
							((t_floatLine*)linia_anterioara)->next = linia_urmatoare;
						else if (t->type == STRING)
							((t_stringLine*)linia_anterioara)->next = linia_urmatoare;
						free(lp);
						lp = linia_urmatoare;
						continue;
					}
				}

				linia_anterioara = lp;

				if (t->type == INT)
					lp = ((t_intLine*)lp)->next;
				else if (t->type == FLOAT)
					lp = ((t_floatLine*)lp)->next;
				else if (t->type == STRING)
					lp = ((t_stringLine*)lp)->next;
			}

		} else if (strcmp(comanda, "CLEAR") == 0) {
			DEBUG && printf("Am primit comanda CLEAR\n");

			// comanda: CLEAR <nume_tabel>
			// gasesc numele tabelului
			p = strtok(NULL, " ");

			// gasesc tabelul in baza de date
			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database.\n", p);
				continue;
			} else {
				freeLinesInTable(t);
			}

		} else if (strcmp(comanda, "ADD") == 0) {
			DEBUG && printf("Am primit comanda ADD\n");

			// comanda: ADD <nume tabel> <valoare celula 1> <valoare celula 2> ...

			// citesc al doilea cuvant: <nume tabel>
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			t_table *t = findTable(&baza_de_date, p);
			if (t == NULL) {
				printf("Table \"%s\" not found in database.\n", p);
				continue;
			}


			if (t->type == INT) {
				DEBUG && printf("Tipul INT \n");

				t_intLine *linie_noua = (t_intLine*)malloc(sizeof(t_intLine));
				linie_noua->next = NULL;
				linie_noua->cells = NULL;

				DEBUG && printf("Am creat linie \n");

				t_intCell *celula_noua;

				DEBUG && printf("Citesc prima celula \n");

				p = strtok(NULL, " ");

				DEBUG && printf("Prima celula este %s \n", p);

				celula_noua = (t_intCell*)malloc(sizeof(t_intCell));

				DEBUG && printf("Am creat celula \n");

				celula_noua->next = NULL;
				celula_noua->value = atoi(p);

				DEBUG && printf("Am setat valorile in celula \n");

				linie_noua->cells = celula_noua;

				DEBUG && printf("Am bagat celula in linie \n");

				t_intCell *ultima_celula = linie_noua->cells;

				DEBUG && printf("Am creat pointer la ultima celula adaugata \n");

				p = strtok(NULL, " ");
				while (p != NULL) {
					DEBUG && printf("Am citit o noua valoare: %s \n", p);

					celula_noua = (t_intCell*)malloc(sizeof(t_intCell));
					celula_noua->next = NULL;
					celula_noua->value = atoi(p);

					DEBUG && printf("Am creat + facut valori pt noua celula \n");

					ultima_celula->next = celula_noua;
					ultima_celula = ultima_celula->next;

					DEBUG && printf("Am bagat noua celula in lista\n");

					p = strtok(NULL, " ");
				}

				DEBUG && printf("Am terminat cu celulele \n");

				if (t->lines == NULL) {
					t->lines = linie_noua;

					DEBUG && printf("Am pus prima linie in tabel \n");
				} else {
					DEBUG && printf("Punem o alta linie, nu prima \n");

					t_intLine *lp;
					lp = t->lines;

					while (lp->next != NULL) {
						DEBUG && printf("Parcurgem \n");
						lp = lp->next;
					}

					lp->next = linie_noua;
					DEBUG && printf("Am adaugat linia \n");
				}
			} else if (t->type == FLOAT) {
				t_floatLine *linie_noua = (t_floatLine*)malloc(sizeof(t_floatLine));
				linie_noua->next = NULL;
				linie_noua->cells = NULL;

				t_floatCell *celula_noua;

				p = strtok(NULL, " ");
				celula_noua = (t_floatCell*)malloc(sizeof(t_floatCell));
				celula_noua->next = NULL;
				celula_noua->value = atof(p);
				linie_noua->cells = celula_noua;

				t_floatCell *ultima_celula = linie_noua->cells;
				p = strtok(NULL, " ");
				while (p != NULL) {
					celula_noua = (t_floatCell*)malloc(sizeof(t_floatCell));
					celula_noua->next = NULL;
					celula_noua->value = atof(p);

					ultima_celula->next = celula_noua;
					ultima_celula = ultima_celula->next;

					p = strtok(NULL, " ");
				}

				if (t->lines == NULL) {
					t->lines = linie_noua;
				} else {
					t_floatLine *lp;
					lp = t->lines;

					while (lp->next != NULL) {
						lp = lp->next;
					}

					lp->next = linie_noua;
				}
			} else if (t->type == STRING) {
				t_stringLine *linie_noua = (t_stringLine*)malloc(sizeof(t_stringLine));
				linie_noua->next = NULL;
				linie_noua->cells = NULL;

				t_stringCell *celula_noua;

				p = strtok(NULL, " ");
				celula_noua = (t_stringCell*)malloc(sizeof(t_stringCell));
				celula_noua->next = NULL;
				char *string_nou = (char *)malloc(sizeof(char)*(strlen(p)+1));
				strcpy(string_nou, p);
				celula_noua->value = string_nou;
				linie_noua->cells = celula_noua;

				t_stringCell *ultima_celula = linie_noua->cells;
				p = strtok(NULL, " ");
				while (p != NULL) {
					celula_noua = (t_stringCell*)malloc(sizeof(t_stringCell));
					celula_noua->next = NULL;
					char *string_nou = (char *)malloc(sizeof(char)*(strlen(p)+1));
					strcpy(string_nou, p);
					celula_noua->value = string_nou;

					ultima_celula->next = celula_noua;
					ultima_celula = ultima_celula->next;

					p = strtok(NULL, " ");
				}

				if (t->lines == NULL) {
					t->lines = linie_noua;
				} else {
					t_stringLine *lp;
					lp = t->lines;

					while (lp->next != NULL) {
						lp = lp->next;
					}

					lp->next = linie_noua;
				}
			} 
			
		} else if (strcmp(comanda, "CREATE") == 0) {
			DEBUG && printf("Am primit comanda CREATE\n");

			// comanda: CREATE <nume tabel> <tip de date> <coloana1> <coloana2> ...

			// citesc al doilea cuvant: <nume tabel>
			p = strtok(NULL, " ");
			strcpy(nume_tabel, p);

			t_table *t = findTable(&baza_de_date, p);
			if (t != NULL) {
				printf("Table \"%s\" already exists.\n", p);
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
				printf("Unknown data type: \"%s\".\n", tip_de_date);
				continue;
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
			printf("Unknown command: \"%s\".\n", comanda);
		}
	}

	return 0;
}