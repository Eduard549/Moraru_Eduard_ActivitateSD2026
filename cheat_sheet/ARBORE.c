#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

// CITIRE DIN FISIER A OBIECTULUI -IDENTICA LA TOATE
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


// afisarea obiectelor din structura

void afisareMasiniDinArbore(Nod* rad) {
	if (rad) {
		afisareMasiniDinArbore(rad->stanga);
		afisareMasina(rad->info);
		afisareMasiniDinArbore(rad->dreapta);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		afisarePreordine(rad->stanga);
		afisarePreordine(rad->dreapta);
	}
}

// citire din fisier si adaugare

void adaugaMasinaInArbore(Nod** rad, Masina masinaNoua) {
	if (*rad==NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->dreapta = NULL;
		nod->stanga = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*rad)->stanga), masinaNoua);
		}
		if ((*rad)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*rad)->dreapta), masinaNoua);
		}
	}

}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			adaugaMasinaInArbore(&rad, m);
		}
	}
	fclose(f);
	return rad;
}

// determina numar noduri + inaltime + get by id

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->stanga) + 
			+ determinaNumarNoduri(rad->dreapta) + 1;
	}
	return 0;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(calculeazaInaltimeArbore(rad->stanga)
			, calculeazaInaltimeArbore(rad->dreapta)) + 1;
	}
	return 0;
}

Masina getMasinaByID(Nod* rad, int id) {
	Masina m;
	m.id = -1;
	if (rad) {
		if (rad->info.id == id) {
			m = rad->info;
			m.model = malloc(sizeof(char) * (strlen(rad->info.model) + 1));
			strcpy(m.model, rad->info.model);
			m.numeSofer = malloc(sizeof(char) * (strlen(rad->info.numeSofer) + 1));
			strcpy(m.numeSofer, rad->info.numeSofer);
		}
		if (id < rad->info.id) {
			m = getMasinaByID(rad->stanga, id);
		}
		if (id > rad->info.id) {
			m = getMasinaByID(rad->dreapta, id);
		}
	}
	return m;
}

int main(){
    Nod* rad = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreordine(rad);

	printf("Masina cautata:");
	afisareMasina(getMasinaByID(rad, 10));

	printf("Numar noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore:%d\n", calculeazaInaltimeArbore(rad));
}


// pt arborele autoechilibrat 

int calculeazaInaltimeArbore(nod* arbore) {
	if (arbore != NULL) {
		int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);
		return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
	}

	return 0;
}

int calculeazaGE(nod* arbore) {
	return calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
}

void rotireStanga(nod** arbore) {
	nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(nod** arbore) {
	nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(nod** arbore, Masina masinaNoua) {
	if ((*arbore) != NULL)
	{
		if ((*arbore)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dreapta), masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->stanga), masinaNoua);
		}
		int gradEchilibru = calculeazaGE(*arbore);
		if (gradEchilibru == 2)
		{
			if (calculeazaGE((*arbore)->stanga) == 1)
			{
				rotireDreapta(arbore);
			}
			else
			{
				rotireStanga(&((*arbore)->stanga));
				rotireDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2)
		{
			if (calculeazaGE((*arbore)->dreapta) != -1)
			{
				rotireDreapta(&((*arbore)->dreapta));
			}
			rotireStanga(arbore);
		}
	}
	else
	{
		(*arbore) = (nod*)malloc(sizeof(nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->stanga = NULL;
		(*arbore)->dreapta = NULL;
	}
}