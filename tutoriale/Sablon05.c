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


struct Nod {
    Masina info;
    struct Nod* prev;
    struct Nod* next;
};
typedef struct Nod Nod;

struct ListaDubla{
    Nod* cap;
    Nod* tail;
    int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

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

// iau nod auxiliar cu adresa capului/cozii si cat timp nu e null, afisez si ma deplasez pe urm/precedentul
void afisareListaMasiniDeLaInceput(ListaDubla lista) {
    Nod* temp = lista.cap;
    while(temp){
        afisareMasina(temp->info);
        temp=temp->next;    
    }
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
    Nod* temp = lista.tail;
    while(temp){
        afisareMasina(temp->info);
        temp=temp->prev;      
    }
}
/*
cream nod nou - alocam memorie + shallow copy/deep copy + asignam next si prev pt noul nod. 
verificam daca tail e null inseamna ca nu avem noduri in lista si trebuie sa asignam pe nodul nostru ca ambele. 
daca avem noduri, facem legatura intre tail si nodul nostru si asignam nodul nou ca tail.

*/
void adaugaMasinaInLista(ListaDubla* lista ,Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info=masinaNoua;
    nodNou->next=NULL;
    nodNou->prev=lista->tail;
    if(lista->tail){
        lista->tail->next=nodNou;
        lista->tail=nodNou;
    }else{
        lista->cap=lista->tail=nodNou;
    }
    lista->nrNoduri++;
}


/*
cream nod nou - alocam memorie + shallow copy/deep copy + asignam next si prev pt noul nod. 
verificam daca cap e null inseamna ca nu avem noduri in lista si trebuie sa asignam pe nodul nostru ca ambele. 
daca avem noduri, facem legatura intre cap si nodul nostru si asignam nodul nou ca cap.

*/
void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info=masinaNoua;
    if(masinaNoua.model){
        nodNou->info.model = (char*)malloc(strlen(masinaNoua.model)+1);
        strcpy(nodNou->info.model, masinaNoua.model);
    }
    if(masinaNoua.numeSofer){
        nodNou->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer)+1);
        strcpy(nodNou->info.numeSofer, masinaNoua.numeSofer);
    }
    nodNou->prev=NULL;
    nodNou->next=lista->cap;
    if(lista->cap){
        lista->cap->prev=nodNou;
        lista->cap=nodNou;
    }else{
        lista->cap=lista->tail=nodNou;
    }
    lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista;
    lista.nrNoduri=0;
    lista.cap=NULL;
    lista.tail=NULL;
    FILE* file=fopen(numeFisier, "r");
    while(!feof(file)){
        adaugaMasinaInLista(&lista,citireMasinaDinFisier(file));
    }
    fclose(file);
    return lista;
}

// imi iau nod auxiliar pe cap actualizez cap ca next, eliberez nodul auxiliar si decrementez nr noduri
void dezalocareLDMasini(ListaDubla* lista) {
	while(lista->cap){
        Nod* p=lista->cap;
        lista->cap=lista->cap->next;
        if(p->info.model){
            free(p->info.model);}
        if(p->info.numeSofer){
            free(p->info.numeSofer);}
        free(p);
        lista->nrNoduri--;
    }
    lista->tail=NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    float suma=0;
    Nod* p=lista.cap;
    while(p){
        suma+=p->info.pret;
        p=p->next;
    }
    if(suma>0){
        return suma/lista.nrNoduri;
    }

    return 0;
}

/*
verificam daca avem noduri , luam aux si verificam pana indeplinim conditia de stergere sau ajungem la final.
daca nu am ajuns la final = nod de sters -> veriicam daca e primul nod / ultimul 
*/
void stergeMasinaDupaID(ListaDubla* lista ,int id) {
    if(!lista->cap){
        return;} // cazul in care nu avem niciun nod in lista 
    Nod* p=lista->cap;
    while(p && p->info.id!=id){
        p=p->next;
        } // ne mutam pana gasim nod sau ajungem la final
    if(!p){
        return;} // cazul in care am ajuns la final
    if(!p->prev){  
        lista->cap=p->next;
        if(lista->cap){lista->cap->prev=NULL;}}    // cazul in care e primul nod de sters    
    else{p->prev->next=p->next;} //legatura dintre p-1 -> p+1
    if(p->next){p->next->prev=p->prev;} //legatura dinre p+1 -> p-1
    else{lista->tail=p->prev; lista->tail->next=NULL;} // cazul in car e ultimul nod de sters
    if(p->info.model){free(p->info.model);} 
        if(p->info.numeSofer){free(p->info.numeSofer);}
    free(p); 
    lista->nrNoduri--;
}
 

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	Nod* max=lista.cap;
    Nod* p=lista.cap;
    while(p){
        if(p->info.pret > max->info.pret){max=p;}
        p=p->next;
    }
    if(max){
        return max->info.numeSofer;
    }
	return NULL;
}

int main() {
    ListaDubla listaMasini=citireLDMasiniDinFisier("masini.txt");
    // afisareListaMasiniDeLaInceput(listaMasini);
    // dezalocareLDMasini(&listaMasini);
    // afisareListaMasiniDeLaInceput(listaMasini);
    // afisareListaMasiniDeLaSfarsit(listaMasini);
    printf("Pret mediu: %.2f\n", calculeazaPretMediu(listaMasini));
	printf("Nume sofer: %s\n", getNumeSoferMasinaScumpa(listaMasini));
	stergeMasinaDupaID(&listaMasini, 9);
    afisareListaMasiniDeLaInceput(listaMasini);
    
    return 0;
}
