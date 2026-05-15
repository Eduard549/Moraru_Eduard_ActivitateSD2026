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

struct Heap {
	int lungime;
	Masina* vector;
	int nrElemente;
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

// afisare 
void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		afisareMasina(heap.vector[i]);
	}
}

// initializare

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.vector = malloc(sizeof(Masina) * lungime);
	heap.nrElemente = 0;
	return heap;
}

// citire din fisier si adaugare in heap

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	Heap heap = initializareHeap(10);
	heap.nrElemente = 0;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			heap.vector[heap.nrElemente++] = citireMasinaDinFisier(f);
		}
	}
	fclose(f);
	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt<heap.vector && heap.vector[pozMax].pret < heap.vector[pozFiuSt].pret) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr<heap.vector && heap.vector[pozMax].pret < heap.vector[pozFiuDr].pret) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

// main

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	return 0;
}