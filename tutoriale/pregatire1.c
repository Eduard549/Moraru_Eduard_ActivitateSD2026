#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Masina{
    int km;
    int an;
    float pret;
    char* numeSofer;
    unsigned char serie;
};
typedef struct Masina Masina;

struct Nod{
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;


void afisareMasina(Masina masina){
    printf("%d\n",masina.km);
    printf("%d\n",masina.an);
    printf("%.2f\n",masina.pret);
    printf("%s\n",masina.numeSofer);
    printf("%c\n\n",masina.serie);
}

void afisareListaMasini(Nod* cap){
    while(cap){
        afisareMasina(cap->info);
            cap=cap->next;
        }
    }


void adaugaInLista(Nod** cap, Masina masinaNoua){
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info=masinaNoua;
    nou->next=NULL;
    if((*cap)){
        Nod* p = *cap;
        while(p->next){
            p=p->next;
        }
        p->next = nou;
    }else{(*cap)=nou;}
}

void adaugaInListaInceput(Nod** cap, Masina masinaNoua){
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = *cap;
    *cap = nou;
}

Masina citireMasinaDinFisier(FILE* file){
    Masina m;
    char buffer[100];
    char sep[4] = ",-\n";
    char* aux;
    fgets(buffer, 100, file);
    m.km = atoi(strtok(buffer, sep));
    m.an = atoi(strtok(NULL, sep));
    m.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m.numeSofer = malloc(strlen(aux)+1);
    strcpy(m.numeSofer, aux);
    m.serie = *strtok(NULL, sep);
    return m;
}

Nod* citireListaMasini(char* numeFisier){
    FILE* file = fopen(numeFisier, "r");
    Nod* cap = NULL;
    while(!feof(file)){
        adaugaInLista(&cap,citireMasinaDinFisier(file));
    }
    fclose(file);
    return cap;
} 

void dezalocareListaMasini(Nod** cap){
    while(*cap){
        Nod* p = *cap;
        *cap = p->next;
        if(p->info.numeSofer){
            free(p->info.numeSofer); 
        }
        free(p);
    }

}   


int main(){
    Nod* nod = citireListaMasini("masini2.txt");
    afisareListaMasini(nod);
    dezalocareListaMasini(&nod);
    afisareListaMasini(nod);
    return 0;
}