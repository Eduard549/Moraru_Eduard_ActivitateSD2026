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
    struct Nod* next;
};

typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[4] = ",-\n";
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
        cap=cap->next;
    }
}

//adaugare la final - alocam memorie pt nod nou, asignam valori nodului, iteram prin lista pana ajungem la ultimul nod, caruia ii asignam adresa nodului nou. daca lista e goala, asignam adresa nod nou ca adresa cap
void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou=(Nod*)malloc(sizeof(Nod));
    nodNou->info=masinaNoua; //shallow
    nodNou->next=NULL;
    if((*cap)){
        Nod* p=*cap;
    while((p->next)){
        p=p->next;
    }
    p->next=nodNou;
    }else{
        *cap=nodNou;
    }
}

// adaugare la inceput - alocam memorie, asignam valori. ca next pt nod nou asignam adresa cap si adresa nod devine adresa cap.
void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou=(Nod*)malloc(sizeof(Nod));
    nodNou->info=masinaNoua;
    nodNou->next=(*cap);
    (*cap)=nodNou;
}

//asignam adresa null pt noul nod, deschidem fisierul, verificam daca exista, iteram pana la flagul end of file , inchidem fisierul si returnam pointerul catre primul nod din lista
Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap=NULL;
    FILE* file=fopen(numeFisier, "r");
    if(file){
        while(!feof(file)){
            adaugaMasinaInLista(&cap,citireMasinaDinFisier(file));  
        }
    }
    
    fclose(file);
    return cap;
}

//stergem de la cap inspre coada -> mutam capul la p+1 si dam free la ce e alocat dinamic in p si dupa dam free la p si tot asa pt fiecare nod pana ajungem la final. pentru ultimul nod, cap devine NULL pt ca adresa ultimului nod e NULL, iese din loop si s-a terminat smecheria.
void dezalocareListaMasini(Nod**cap) {
    while(*cap){
        Nod* p=*cap;
        (*cap)=p->next; //  p->next = (*cap)->next;
        if(p->info.model){
            free(p->info.model);}
        if(p->info.numeSofer){
            free(p->info.numeSofer);}
        free(p);
    }

}


// mut capul pe urmatorul si pe fiecare fac operatiile de care am nevoie
float calculeazaPretMediu(Nod* cap) {
    float suma=0;
    int i=0;
     while(cap){
        suma+= cap->info.pret;
        cap=cap->next;
        i++;
    }
    if(i > 0){
        return suma / i;
    }
    return 0;
}


/*
daca primul e de sters -> mut capul la al doilea si dupa sterg primul nod, daca e null, se opreste automat.
daca ultimul nod trebuie sters -> 
*/

void stergeMasiniDinSeria(Nod** cap ,char serieCautata) {
   
/*
    prima oara verificam de la inceput pana la primul item care nu trebuie sters.
    pt nodurile care trebuie sterse:
        ->ne luam nod auxiliar cu adresa capului 
        ->setam cap urmatorul nod
        ->stergem nodul
    verificam daca au ramas noduri - daca cap nu e null
        -> ne luam nod auxiliar cu adresa capului
        -> cat timp nodul auxiliar nu e null
            -> cat timp nodul urmator celui auxiliar nu e null si nu trebuie sters, avansam cu nod auxiliar = nod urmator:
            -> am iesit din while daca am ajuns la final si nu am intalnit noduri de sters sau daca am intalnit noduri de sters
                -> verificam daca nodul urmator celui auxiliar nu e null, inseamna ca trebuie sters
                    -> ne luam nod auxiliar din nou cu valoarea nodului urmator celui care trebuie sters
                    ->nodul auxiliar precedent isi ia valoarea nodului auxiliar pe care l-am asignat
                    ->stergem nodul auxiliar pe care l-am asignat
                -> daca e null, asignam nodul auxiliar ca NULL si iese din loop, a ajuns la final.     


*/
    while((*cap) && (*cap)->info.serie==serieCautata){
        Nod* p=*cap;    
        *cap=(*cap)->next;
        if(p->info.model){free(p->info.model);}
        if(p->info.numeSofer){free(p->info.numeSofer);}
        free(p);
        }
    if((*cap)){
        Nod* q = *cap;
        while(q){
    while(q->next && q->next->info.serie!= serieCautata){
       q = q->next; 
    }
    if(q->next){
        Nod* aux = q->next;
        q->next = aux->next;
        if(aux->info.model){free(aux->info.model);}
        if(aux->info.numeSofer){free(aux->info.numeSofer);}
        free(aux);
        
            }else{
                q=NULL;
            }
        }
    }
    
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap ,const char* numeSofer) {
	float suma=0;
    while(cap){
        if(strcmp(cap->info.numeSofer, numeSofer)==0){
        suma+= cap->info.pret;}
        cap=cap->next;
    }
	return suma;
}

int main() {
    Nod* cap = citireListaMasiniDinFisier("masini.txt");
    // afisareListaMasini(cap);
    // dezalocareListaMasini(&cap);
    // printf("Pret mediu: %.2f\n",calculeazaPretMediu(cap));
    // char* numeSofer = "Ionescu";
    // printf("Suma masini %s: %.2f\n", numeSofer, calculeazaPretulMasinilorUnuiSofer(cap, numeSofer));
    stergeMasiniDinSeria(&cap, 'A');
    afisareListaMasini(cap);
	return 0;
}