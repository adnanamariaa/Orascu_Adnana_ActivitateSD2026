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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrMasini;
	Masina* masini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.masini = malloc(lungime * sizeof(Masina));
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	if (pozitieNod >= 0 && pozitieNod < heap.nrMasini) {
		int pmax = pozitieNod;
		int pozStanga = pozitieNod * 2 + 1;
		int pozDreapta = pozitieNod * 2 + 2;
		if (pozStanga < heap.nrMasini && heap.masini[pmax].pret < heap.masini[pozStanga].pret) {
			pmax = pozStanga;
		}
		if (pozDreapta < heap.nrMasini && heap.masini[pmax].pret < heap.masini[pozDreapta].pret) {
			pmax = pozDreapta;
		}
		if (pmax != pozitieNod) {
			Masina aux = heap.masini[pmax];
			heap.masini[pmax] = heap.masini[pozitieNod];
			heap.masini[pozitieNod] = aux;
			if (pmax < (heap.nrMasini - 2) / 2)
				filtreazaHeap(heap, pmax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	heap.nrMasini = 0;

	while (!feof(file)) {
		heap.masini[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);

	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--) { //incepem direct cu ultimul nod care este parinte
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrMasini; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	if (heap->nrMasini <= 0) {
		Masina m;
		m.id = -1;
		return m;
	}
	Masina aux = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrMasini - 1];
	heap->masini[heap->nrMasini - 1] = aux;

	heap->nrMasini--;

	for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*heap, i);
	}
	return heap->masini[heap->nrMasini];
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(h);
	return 0;
}