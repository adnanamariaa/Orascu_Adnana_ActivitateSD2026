#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita

typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;


};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	nou->next = NULL;
	Nod* aux = *lista;
	if ((*lista) != NULL)
	{
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*lista = nou;
	}
}

void afisareListaMasini(Nod *cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *lista;

	*lista = nou;
}

//o functie void* returneaza un pointer la orice, poate fi pus la orice functie care returneaza un pointer dar creaza ambiguitate
Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f))
	{
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	//sunt dezalocate toate masinile si lista de elemente
	Nod* head = *lista;
	Nod* next = NULL;
	while (head != NULL)
	{
		next = head->next;
		free(head->info.model);
		free(head->info.numeSofer);
		free(head);
		head = next;
	}
	*lista = NULL;
}

float calculeazaPretMediu(Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int nrMasini = 0;
	while (lista != NULL)
	{
		suma += lista->info.pret;
		nrMasini++;
		lista = lista->next;
	}
	if(nrMasini!=0)
		return (suma/nrMasini);
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		if (aux->info.model) {
			free(aux->info.model);
		}
		free(aux);
	}
	if ((*cap)) {
		Nod* p = *cap;
		while (p) {
			while (p && p->next && p->next->info.serie != serieCautata) {
				p = p->next;
			}
			if (p->next) {
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer) {
					free(aux->info.numeSofer);
				}
				if (aux->info.model) {
					free(aux->info.model);
				}
				free(aux);
			}
			else {
				p = NULL;
			}
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}

		cap = cap->next;
	}
	return suma;
}

int main() {

	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	printf("Afisarea listei:\n");
	afisareListaMasini(lista);
	
	float pretMediu = calculeazaPretMediu(lista);
	printf("\nPretul mediu este: %3.2f\n", pretMediu);

	FILE* f = fopen("masini.txt", "r");
	if (f != NULL) {
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
		fclose(f);
	}
	printf("\nAfisarea listei dupa adaugarea elementului la inceout:\n");
	afisareListaMasini(lista);

	printf("\nPretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(lista, "Gigel"));

	printf("\nStergere seria A:\n");
	stergeMasiniDinSeria(&lista, 'A');
	afisareListaMasini(lista);

	printf("\nStergere seria B:\n");
	stergeMasiniDinSeria(&lista, 'B');
	afisareListaMasini(lista);

	dezalocareListaMasini(&lista);
	printf("\nDupa dezalocare:\n");
	afisareListaMasini(lista);

	return 0;
}