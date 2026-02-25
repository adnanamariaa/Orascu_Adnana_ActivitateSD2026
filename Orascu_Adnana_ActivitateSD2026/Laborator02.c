#include<stdio.h>
#include<malloc.h>

struct Film {
	int id;
	int durata;
	char* denumire;
	float buget;
	char varstaMinima;
};

struct Film initializare(int _id, int _durata, const char* _denumire, float _buget, char _varstaMinima) {
	struct Film s;

	s.id = _id;
	s.durata = _durata;
	s.denumire = malloc((strlen(_denumire) + 1) * sizeof(char));
	strcpy(s.denumire, _denumire);
	s.buget = _buget;
	s.varstaMinima = _varstaMinima;

	return s;
}

struct Film copyFilm(struct Film s)
{
	return initializare(s.id, s.durata, s.denumire, s.buget, s.varstaMinima);
}

void afisare(struct Film s) {
	printf("Id: %d\n", s.id);
	printf("Nume: %s\n", s.denumire);
	printf("Durata in minute: %d\n", s.durata);
	printf("Buget: %2f\n", s.buget);
	printf("Varsta minima: %d\n\n", s.varstaMinima);
}

void afisareVector(struct Film* vector, int nrFilme) {
	for (int i = 0; i < nrFilme; i++)
		afisare(vector[i]);
}

struct Film* copiazaPrimeleNElemente(struct Film* vector, int nrFilme, int nrFilmeCopiate) {

	struct Film* vectorNou = malloc(nrFilmeCopiate * sizeof(struct Film));
	for (int i = 0; i < nrFilmeCopiate; ++i)
	{
		vectorNou[i] = copyFilm(vector[i]);
	}

	return vectorNou;
}

void dezalocare(struct Film** vector, int* nrFilme) {
	for (int i = 0; i < *(nrFilme); i++)
		free((*vector)[i].denumire);
	free(*vector);

	*vector = NULL;
	*nrFilme = 0;
}

void copiazaAnumiteElemente(struct Film* vector, char nrFilme, float bugetMaxim, struct Film** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrFilme; i++)
		if (vector[i].buget < bugetMaxim)
			(*dimensiune)++;
	(*vectorNou) = malloc((*dimensiune) * sizeof(struct Film));

	int k = 0;
	for (int i = 0; i < nrFilme; i++)
	{
		if (vector[i].buget < bugetMaxim)
		{
			(*vectorNou)[k] = copyFilm(vector[i]);
			k++;
		}
	}
}

struct Film getPrimulElementConditionat(struct Film* vector, int nrFilme, const char* numeCautat) {

	struct Film s;
	s.id = -1;
	s.denumire = NULL;
	for (int i = 0; i < nrFilme; i++)
		if (!(strcmp(vector[i].denumire, numeCautat)))
			return vector[i];

	return s;
}



int main() {
	struct Film f1 = initializare(1, 120, "Dune", 20.6, 14);
	afisare(f1);

	int nrFilme = 3;
	struct Film* filme = malloc(nrFilme * sizeof(struct Film));
	filme[0] = f1;
	*(filme + 1) = initializare(2, 360, "James Bond", 16, 12);
	filme[2] = initializare(3, 240, "Star Wars", 50.5, 12);
	//index face deplasare si dereferentiere
	afisareVector(filme, nrFilme);

	int nrFilmeCopiate = 2;
	struct Film* vectorNou;
	vectorNou = copiazaPrimeleNElemente(filme, nrFilme, nrFilmeCopiate);
	printf("\n\nElemente copiate:\n");
	afisareVector(vectorNou, nrFilmeCopiate);

	dezalocare(&vectorNou, &nrFilmeCopiate);
	printf("\n\nElemente copiate:\n");
	afisareVector(vectorNou, nrFilmeCopiate);

	struct Film* vectorIeftin;
	float prag = 50;
	int dimIeftin;
	copiazaAnumiteElemente(filme, nrFilme, prag, &vectorIeftin, &dimIeftin);
	afisareVector(vectorIeftin, dimIeftin);

	afisare(getPrimulElementConditionat(filme, nrFilme, "Dune"));
	dezalocare(&filme, &nrFilme);

	return 0;
}