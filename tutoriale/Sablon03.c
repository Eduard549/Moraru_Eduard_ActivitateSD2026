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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
    printf("Nr usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for(int i=0;i<nrMasini;i++){
        afisareMasina(masini[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
    Masina* masiniNou=(Masina*)malloc(sizeof(Masina)*((*nrMasini)+1));
    for(int i=0;i<(*nrMasini);i++){
       masiniNou[i] = (*masini)[i];
    }
    masiniNou[(*nrMasini)] = masinaNoua;
    (*nrMasini)++;//daca nu dereferentiem cu () ++ se aplica pointerului, nu valorii
    free(*masini);
    (*masini) = masiniNou;
}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
    char buffer[100];
    char separator[4] = ",;\n";
    fgets(buffer, 100, file);
    m.id = atoi(strtok(buffer, separator));//atoi - ascii to integer - parametru un char pointer
    m.nrUsi = atoi(strtok(NULL, separator));
    m.pret = atof(strtok(NULL, separator));
    char* aux; 
    aux= strtok(NULL, separator);
    m.model = (char*)malloc(strlen(aux)+1);
    strcpy(m.model, aux);
    aux= strtok(NULL, separator);
    m.numeSofer = (char*)malloc(strlen(aux)+1);
    strcpy(m.numeSofer, aux);
    m.serie = strtok(NULL, separator)[0];
    return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
    FILE* file=fopen(numeFisier, "r");
    Masina* masini = NULL;
    (*nrMasiniCitite)=0;
    while(!feof(file)){
        adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
    }
    fclose(file);
    return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		if ((*vector)[i].model != NULL) {
			free((*vector)[i].model);
		}
		if ((*vector)[i].numeSofer != NULL) {
			free((*vector)[i].numeSofer);
		}
}
}

int main() {
    int nrMasini=0;
    Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
    afisareVectorMasini(masini, nrMasini);
    dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}