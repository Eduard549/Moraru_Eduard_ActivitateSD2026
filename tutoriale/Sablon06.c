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

struct Nod{
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;


struct HashTable {
	int dim;
    Nod** tabela;
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

void afisareListaMasini(Nod* cap) {
    while(cap){
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) {
    Nod* p = cap;
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next=NULL;
    while(p->next){
        p=p->next;
    }
    p->next=nou;
    
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
    ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
    for(int i=0;i<dimensiune;i++){
        ht.tabela[i] = NULL;
    }
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) {
	int suma=0;
    for(int i=0;i<strlen(numeSofer);i++){
        suma+=numeSofer[i];
    }
    return suma%dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
    if(hash.tabela[pozitie]){
        adaugaMasinaInLista(hash.tabela[pozitie], masina);
    }
    else{
        hash.tabela[pozitie] = malloc(sizeof(Nod));
        hash.tabela[pozitie]->info=masina;
        hash.tabela[pozitie]->next=NULL;
    }
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
    FILE* file = fopen(numeFisier, "r");
    HashTable nou=initializareHashTable(dimensiune);
    while (!feof(file)){
       inserareMasinaInTabela(nou, citireMasinaDinFisier(file));
    }
    fclose(file);
    return nou;
}

void afisareTabelaDeMasini(HashTable ht) {
    for(int i=0; i<ht.dim;i++){
        if(ht.tabela[i]){
            printf("masinile de pe pozitia %d sunt:\n",i);
            afisareListaMasini(ht.tabela[i]);
        }
    }
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for(int i=0;i<ht->dim;i++){
            Nod* p = ht->tabela[i];
            while(p){
                Nod* aux = p;
                p=p->next;
                if(aux->info.model){
                    free(aux->info.model);
                }
                if(aux->info.numeSofer){
                    free(aux->info.numeSofer);
                }
                free(aux);
            }
            ht->tabela[i] = NULL;
        
    }
    ht->dim=0;
    free(ht->tabela);
    ht->tabela = NULL;
}
float calculeazaMedieLista(Nod* cap){
    float suma=0;
    int nrElemente=0;
    while(cap){
        suma+=cap->info.pret;
        nrElemente++;
        cap=cap->next;
    }
    return (nrElemente > 0 ?(suma/nrElemente):0);
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
    *nrClustere=0;
    float* preturi = NULL;
    for(int i=0;i<ht.dim;i++){
        if(ht.tabela[i]){
            (*nrClustere)++;
        }
    }
    preturi = malloc(sizeof(float)* (*nrClustere));
    int index =0;
    for(int i=0;i<ht.dim;i++){
        preturi[index]=calculeazaMedieLista(ht.tabela[i]);
        index++;
    }
	return preturi;
}



Masina getMasinaDinLista(Nod* cap, const char* numeSofer){
    Masina m;
    m.id=-1;
    while(cap&& strcmp(numeSofer, cap->info.numeSofer)!=0){
        cap = cap->next;

    }
    if(cap){
        m=cap->info;
        m.model = (char*)malloc(strlen(cap->info.model)+1);
        strcpy(m.model, cap->info.model);
        m.numeSofer = (char*)malloc(strlen(cap->info.numeSofer)+1);
        strcpy(m.numeSofer, cap->info.numeSofer);
    }
    return m;

}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
    int index = calculeazaHash(numeSofer, ht.dim);
    if(index>=0 && index < ht.dim){
        return getMasinaDinLista(ht.tabela[index], numeSofer);
    }


	return m;
}

int main() {
    int nrClustere;
    HashTable hash = citireMasiniDinFisier("masini3.txt", 10);
    // afisareTabelaDeMasini(hash);
    // dezalocareTabelaDeMasini(&hash);
    // afisareTabelaDeMasini(hash);
    nrClustere = 0;
    float* preturi = calculeazaPreturiMediiPerClustere(hash, &nrClustere);
    printf("nr clustere: %d\n", nrClustere);
    for(int i=0;i<nrClustere; i++){
        printf("cluster %d: %.2f\n", i+1,preturi[i]);
    }
    afisareMasina(getMasinaDupaCheie(hash, "Gigel Alex"));
    dezalocareTabelaDeMasini(&hash);
	return 0;
}