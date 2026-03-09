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
typedef struct Nod Nod; // alias
typedef struct ListaDubla ListaDubla;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};


//creare structura pentru Lista Dubla 
struct ListaDubla{ //aici retin primul si ultimul nod pointers
	Nod* prim;
	Nod* ultim;
};


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

void afisareListaMasini(ListaDubla lista/*lista dubla de masini*/) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p =lista.prim;
	while(p){
		afisareMasina(p->info);
		p=p->next;
	}
}
void afisareInversaListaMasini(ListaDubla lista/*lista dubla de masini*/) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p =lista.ultim;
	while(p){
		afisareMasina(p->info);
		p=p->prev;
	}
}



void adaugaMasinaInLista(ListaDubla* lista, /*lista dubla de masini*/ Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod * nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = nou->prev=NULL;
	if(lista->ultim){
		nou->prev=lista->ultim;
		nou->next=NULL;
		lista->ultim->next=nou;
		lista->ultim=nou;
	}
}

void adaugaLaInceputInLista(ListaDubla* lista,/*lista dubla de masini*/ Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod * nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = nou->prev=NULL;
	if(lista->prim){
		nou->next=lista->prim;
		nou->prev=NULL;
		lista->prim->prev=nou;
		lista->prim=nou;
	}else{

	}
}

ListaDubla* citireLDMasiniDinFisier(char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file=fopen(numeFisier, "r");
	if(file){
		ListaDubla *lista=malloc(sizeof(ListaDubla));
		lista->prim=NULL;
		lista->ultim=NULL;
		while(!feof(file)){
			adaugaMasinaInLista(*lista, citireMasinaDinFisier(file));
		}
	
	fclose(file);
	return lista;
} else{
	ListaDubla* lista=malloc(sizeof(ListaDubla));
	lista->prim=NULL;
	lista->ultim=NULL;
	fclose(file);
	return lista;
}
}


void dezalocareLDMasini(/*lista dubla de masini*/) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
}

float calculeazaPretMediu(/*lista de masini*/) {
	//calculeaza pretul mediu al masinilor din lista.
	return 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	ListaDubla* lista = citireLDMasiniDinFisier("masini.txt");
	afisareInversaListaMasini(*lista);

	return 0;
}