#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Masina {
	int km;
    int an;
    float pret;
    char* numeSofer;
    unsigned char serie;
};

typedef struct Masina Masina;

struct Nod{
    Masina info;
    struct Nod* prev;
    struct Nod* next;
};
typedef struct Nod Nod;

struct ListaMasini{
    Nod* cap;
    Nod* end;
    int nrNoduri;
};
typedef struct ListaMasini ListaMasini;

void afisareMasina(Masina masina){
    printf("%d\n",masina.km);
    printf("%d\n",masina.an);
    printf("%.2f\n",masina.pret);
    printf("%s\n",masina.numeSofer);
    printf("%c\n\n",masina.serie);
}

void afisareListaMasini(ListaMasini lista){
    Nod* p = lista.cap;
    while(p){
        afisareMasina(p->info);
        p = p->next;
    }
}

void adaugaMasinaInLista(ListaMasini* lista, Masina masinaNoua){
    Nod* nou = malloc(sizeof(Nod));
    nou->info=masinaNoua;
    nou->next=NULL;
    nou->prev=lista->end;
    if(lista->end){
        lista->end->next = nou;
        lista->end = nou;
    }
    else{
        lista->cap = nou;
        lista->end = nou;
    }
    lista->nrNoduri++;
}

void adaugaMasinaInListaInceput(ListaMasini* lista, Masina masinaNoua){
    Nod* nou = malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->prev=NULL;
    nou->next=lista->cap;
    if(lista->cap){
        lista->cap->prev=nou;
        lista->cap = nou;
    }else{
        lista->cap=nou;
        lista->end=nou;
    }
    lista->nrNoduri++;
}

Masina citireMasinaDinFisier(FILE* file){
    char buffer[100];
    char sep[4]=",-\n";
    char* aux;
    Masina m;    
    fgets(buffer,100,file);

    m.km=atoi(strtok(buffer, sep));
    m.an=atoi(strtok(NULL, sep));
    m.pret=atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m.numeSofer = malloc(strlen(aux)+1);
    strcpy(m.numeSofer, aux);
    m.serie = *strtok(NULL, sep);
    return m;
}

ListaMasini citireListaMasiniDinFisier(char* numeFisier){
    ListaMasini lista;
    lista.nrNoduri=0;
    lista.end=NULL;
    lista.cap=NULL;
    FILE* file = fopen(numeFisier, "r");
    while(!feof(file)){
        adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
    }
    fclose(file);
    return lista;
}

void dezalocareListaMasini(ListaMasini* lista){
    while(lista->cap){
        Nod* p = lista->cap;
        lista->cap=lista->cap->next;
        if(p->info.numeSofer){free(p->info.numeSofer);}
        free(p);
        lista->nrNoduri--;
    }
    lista->end=NULL;
}

float calculeazaTotalMasini(ListaMasini lista){
    float suma=0;
    Nod* p = lista.cap;
    while(p){
        suma+=p->info.pret;
        p=p->next;
    }
    return suma; 
}


/*
Lista goală → ieșire imediată

Căutarea nodului după nume

Nodul nu există → ieșire

Ștergere primul nod (actualizare cap și cap->prev sau end dacă lista devine goală)

Ștergere nod din mijloc (legătura prev și next)

Ștergere ultim nod (actualizare end și end->next)

Eliberarea memoriei (numeSofer + nod)

Actualizarea numărului de noduri (nrNoduri--)
*/

void stergeMasinaDinLista(ListaMasini* lista, char* numeSofer){
    if(!lista->cap){
        return;
    }
    Nod* p = lista->cap;
    while(p && strcmp(numeSofer, p->info.numeSofer)!=0){
        p=p->next;
    }
    if(!p){return;}
    if(!p->prev){
        lista->cap=p->next;
        if(lista->cap){
            lista->cap->prev=NULL;
        }
        else{
            lista->end=NULL;
        }

    }
    else{
        p->prev->next = p->next;
        if(p->next){
            p->next->prev = p->prev;
    }
        else{
            lista->end=p->prev;
            lista->end->next=NULL;
        }
    }
    if(p->info.numeSofer){
        free(p->info.numeSofer);
    }
    free(p);
    lista->nrNoduri--;
}


int main(){
    ListaMasini lista = citireListaMasiniDinFisier("masini2.txt");
    // afisareListaMasini(lista);
    // dezalocareListaMasini(&lista);
    // afisareListaMasini(lista);
    printf("total: %.2f\n", calculeazaTotalMasini(lista));
    return 0;
}