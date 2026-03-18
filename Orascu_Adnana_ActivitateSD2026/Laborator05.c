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
typedef struct ListaDubla ListaDubla;
typedef struct Nod Nod;

//creare structura pentru un nod dintr-o lista dublu inlantuita

struct Nod {
	Masina info;
	Nod* prev;
	Nod* next;
};

//creare structura pentru Lista Dubla 
struct ListaDubla {
	Nod* prim;
	Nod* ultim;
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
	m1.pret = atof(strtok(NULL, sep));
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

void afisareListaMasini(ListaDubla lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}

}
void afisareInversaListaMasini(ListaDubla lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}

}

void adaugaMasinaInLista(ListaDubla *ld, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim)
		ld->ultim->next = nou;
	else
		ld->prim = nou;
	ld->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* ld, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	if (ld->prim)
		ld->prim->prev = nou;
	else
		ld->ultim = nou;
	ld->prim = nou;

}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}
}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim && ld->prim->next) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla ld) {
	float suma=0;
	int nr=0;

	while (ld.prim)
	{
		nr++;
		suma += ld.prim->info.pret;
		ld.prim = ld.prim->next;

	}
	if (nr)
		return suma / nr;
	else
		return 0;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
	Nod* p = ld->prim;
	while (p && p->info.id != id) p = p->next;

	if (p) {
		if (p->prev)
			p->prev->next = p->next;
		else
			ld->prim = p->next; // daca era primul, noul prim e urm

		if (p->next)
			p->next->prev = p->prev;
		else
			ld->ultim = p->prev; // daca era ultimul, noul ultim e ant

		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla ld) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	char* nume=NULL;
	float pretMax = -1;
	Nod* primulEL=ld.prim;
	while (ld.prim) {
		if (ld.prim->info.pret > pretMax)
			pretMax = ld.prim->info.pret;
		ld.prim = ld.prim->next;
	}

	ld.prim = primulEL;
	while (ld.prim) {
		if (ld.prim->info.pret == pretMax)
		{
			nume = malloc((strlen(ld.prim->info.numeSofer)+1)*sizeof(char));
			strcpy(nume, ld.prim->info.numeSofer);
		}
		ld.prim = ld.prim->next;
	}

	if (nume)
	{
		return nume;
	}
	else
		return NULL;
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	float s = calculeazaPretMediu(lista);
	printf("\nPretul mediu al masinilor este: %.2f", s);

	char* numeSoferMasinaScumpa = getNumeSoferMasinaScumpa(lista);
	printf("\nNumele soferului cu cea mai scumpa masina este: %s\n", numeSoferMasinaScumpa);

	printf("\nMasini dupa stergere:\n");
	stergeMasinaDupaID(&lista, 9);
	afisareListaMasini(lista);

	return 0;
}