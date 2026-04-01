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
	Masina masina;
	Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	Nod** head;//o denumire mai corecta ar fi fost: vector, tabela //de fapt, head e un vector de pointeri
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* nod) {
	while (nod)
	{
		afisareMasina(nod->masina);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(Nod** nod, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	if (!(*nod)) {
		*nod = nou;
	}
	else {
		Nod* aux = *nod;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	ht.dim = dimensiune;
	ht.head = malloc(sizeof(Nod*) * ht.dim);
	for (int i = 0; i < ht.dim; i++)
	{
		ht.head[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii(aici, id)
	return (id * 31) % dimensiune;//nr prim pt a reduce probabilitatea de coliziune

}

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int hash = calculeazaHash(masina.id, ht.dim);

	if (ht.head[hash] != NULL) {
		adaugaMasinaInLista(&(ht.head[hash]), masina);

		return;
	}

	Nod* nod = malloc(sizeof(Nod));
	nod->masina = masina;
	nod->next = NULL;

	ht.head[hash] = nod;
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, masina);
	}

	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			printf("Clusterul %d:\n", i + 1);

			Nod* p = ht.head[i];
			afisareListaMasini(p);
			printf("========================\n");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
	for (int i = 0; i < ht->dim; i++) {

		if (ht->head[i] != NULL){
			Nod* p = ht->head[i];
			Nod* next = NULL;

			while (p != NULL) {
				free(p->masina.model);
				free(p->masina.numeSofer);

				next = p->next;
				free(p);
				p = next;
			}
		}
	}
	free(ht->head);
	ht->dim = 0;
	ht->head = NULL;

}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini

	int nr = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			nr++;
		}
	}

	float* mediePerCluster = malloc(nr * sizeof(float));
	int j = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL) {
			int nrNoduri = 0;
			float sum = 0;

			Nod* aux = ht.head[i];
			while (aux != NULL) {
				nrNoduri++;
				sum += aux->masina.pret;

				aux = aux->next;
			}

			mediePerCluster[j++] = sum / nrNoduri;
		}
	}

	*nrClustere = nr;
	return mediePerCluster;
}

Masina getMasinaById(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	int hash = calculeazaHash(id, ht.dim);

	Nod* aux = ht.head[hash];
	while (aux != NULL) {
		if (aux->masina.id == id) {
			m = aux->masina;

			m.model = malloc((strlen(aux->masina.model) + 1) * sizeof(char));
			strcpy(m.model, aux->masina.model);

			m.numeSofer = malloc((strlen(aux->masina.numeSofer) + 1) * sizeof(char));
			strcpy(m.numeSofer, aux->masina.numeSofer);

			return m;
		}
		aux = aux->next;
	}

	return m;
}

int main() {

	HashTable ht = citireMasiniDinFisier("masini.txt");
	//afisareTabelaDeMasini(ht);

	//afisareMasina(getMasinaById(ht, 9));

	int nrClustere;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);

	for (int i = 0; i < nrClustere; i++)
	{
		printf("%.2f\n", medii[i]);
	}
	dezalocareTabelaDeMasini(&ht);

	return 0;
}