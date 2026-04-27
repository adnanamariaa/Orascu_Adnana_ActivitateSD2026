#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Moneda {
	char* taraEmitenta;
	int valoare;
	float greutate;
	int anEmitere;
};

typedef struct Moneda Moneda;
typedef struct Nod Nod;
struct Nod {
	Moneda info;
	Nod* next;
};

Moneda initializare(const char* taraEmitenta, int valoare, float greutate, int anEmitere) {
	Moneda m;

	if (strlen(taraEmitenta) > 0) {
		m.taraEmitenta = malloc(strlen(taraEmitenta) + 1);
		strcpy(m.taraEmitenta, taraEmitenta);
	}
	m.valoare = valoare;
	m.greutate = greutate;
	m.anEmitere = anEmitere;

	return m;
}
void afisareMoneda(Moneda m) {
	printf("Tara emitenta: %s\n", m.taraEmitenta);
	printf("Valoare nominala: %d\n", m.valoare);
	printf("Greutate: %.2f\n", m.greutate);
	printf("An emitere: %d\n\n", m.anEmitere);
}

void adaugareStiva(Nod** cap, Moneda m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->next = *cap;
	nou->info = m;
	*cap = nou;
}

void afisareStiva(Nod* cap) {
	while (cap) {
		afisareMoneda(cap->info);
		cap = cap->next;
	}
}

Moneda monedaGreutateMax(Nod* cap) {
	float gMax = cap->info.greutate;
	Nod* aux = cap;
	while (aux) {
		if (aux->info.greutate > gMax)
			gMax = aux->info.greutate;
		aux = aux->next;
	}

	Moneda m;
	m.valoare = -1;
	aux = cap;
	while (aux) {
		if (aux->info.greutate == gMax) {
			m = aux->info;
			m.taraEmitenta = malloc(strlen(aux->info.taraEmitenta) + 1);
			strcpy(m.taraEmitenta, aux->info.taraEmitenta);
		}
		aux = aux->next;
	}
	return m;
}
void stergeMonedaDupaGreutate(Nod** cap, float greutate) {
	if (*cap == NULL) return;//nu are noduri

	Nod* anterior = NULL;
	Nod* aux = *cap;

	while (aux) {
		if (aux->info.greutate == greutate) {
			if (anterior == NULL)//era primul nod
				*cap = aux->next;
			else
				anterior->next = aux->next;//era la mijloc
			

			Nod* deSters = aux;
			aux = aux->next;
			free(deSters->info.taraEmitenta);
			free(deSters);
			// free
		}
		else {
			anterior = aux;
			aux = aux->next;
		}
	}
}

Moneda monedaGreutateMinima(Nod* cap) {
	Nod* aux = cap;
	float grMin = aux->info.greutate;

	while (aux) {
		if (grMin > aux->info.greutate) {
			grMin = aux->info.greutate;
		}
		aux = aux->next;
	}

	aux = cap;
	Moneda m;
	while (aux) {
		if (grMin == aux->info.greutate) {
			m = aux->info;
			m.taraEmitenta = malloc(strlen(aux->info.taraEmitenta) + 1);
			strcpy(m.taraEmitenta, aux->info.taraEmitenta);
		}
		aux = aux->next;
	}

	return m;
}

int numaraMonedeDupaAnEmitere(Nod* cap, int an) {
	Nod* aux = cap;
	int nr = 0;

	while (aux) {
		if (aux->info.anEmitere > an)
			nr++;
		aux = aux->next;
	}

	return nr;
}

void stergePrimaMonedaDupaValoare(Nod** cap, int valoare) {
	if (*cap == NULL) return;

	Nod* anterior = NULL;
	Nod* aux = *cap;

	while (aux) {
		if (aux->info.valoare == valoare) {
			if (anterior == NULL) {//nodul de sters era chiar capul stivei
				*cap = aux->next;
			}
			else {//nodul e in interior
				anterior->next = aux->next; // sare peste nodul de sters
			}
			Nod* deSters = aux;
			aux = aux->next;
			free(deSters->info.taraEmitenta);
			free(deSters);
			return;
		}
		else {
			anterior = aux;
			aux = aux->next;
		}
	}


}

int main() {

	Moneda m1 = initializare("Romania", 5, 1.2, 2020);
	Moneda m2 = initializare("Bulgaria", 3, 1.3, 2007);
	Moneda m3 = initializare("Grecia", 10, 0.9, 2018);
	Moneda m4 = initializare("Belgia", 4, 1.5, 2019);
	Moneda m5 = initializare("Romania", 10, 1.3, 2020);
	//afisareMoneda(m1);
	Nod* stiva;
	stiva = NULL;
	adaugareStiva(&stiva, m1);
	adaugareStiva(&stiva, m2);
	adaugareStiva(&stiva, m3);
	adaugareStiva(&stiva, m4);
	adaugareStiva(&stiva, m5);
	afisareStiva(stiva);

	Moneda grMax = monedaGreutateMax(stiva);
	printf("Moneda cu greutatea maxima este:\n");
	afisareMoneda(grMax);

	Moneda grMin = monedaGreutateMinima(stiva);
	printf("Moneda cu greutatea minima este:\n");
	afisareMoneda(grMin);

	printf("Monedele care au fost emise dupa anul 2018, sunt in numar de: %d\n\n", numaraMonedeDupaAnEmitere(stiva, 2018));

	stergePrimaMonedaDupaValoare(&stiva, 10);
	printf("Afisarea stivei dupa stergerea primei aparitii a monedei cu valoarea 10:\n\n");
	afisareStiva(stiva);

	float greutate = 0.9;
	stergeMonedaDupaGreutate(&stiva, greutate);
	printf("Afisarea stivei dupa stergerea monedei cu greutatea %.2f este:\n", greutate);
	afisareStiva(stiva);

	return 0;
}