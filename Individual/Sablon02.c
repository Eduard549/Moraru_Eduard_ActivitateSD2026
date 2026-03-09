#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* dereferentierea se face cu -> de ex daca avem char* lista: ca sa accesam valoarea lui pret avem t->pret, daca lucram cu valoare obiect nu cu referinta, atunci folosim t.pret
   in general, cand lucram cu structuri, folosim referinte la setteri, la getteri nu e nevoie. 
*/

struct Telefon {
	int id;
	int RAM;
    char* producator;
    float pret;
    char serie;
};

struct Telefon initializare(int id, int ram, const char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
    t.RAM = ram;
    t.producator = (char*) malloc(sizeof(char)* (strlen(producator)+1));
    strcpy_s(t.producator, strlen(producator)+1, producator);
    t.pret = pret;
    t.serie = serie;
	return t;
}

void afisare(struct Telefon t) {
	printf("%d.Telefonul %s seria %c are %d Gb RAM si costa %5.2f RON.\n",t.id, t.producator, t.serie, t.RAM, t.pret);
}

void afisareVector(struct Telefon* vector, int nrElemente) {
	for(int i=0; i< nrElemente; i++){
        afisare(vector[i]);
    }
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Telefon *vectorNou=NULL;
    vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon)*nrElementeCopiate);
    for(int i=0; i< nrElementeCopiate;i++){
        vectorNou[i]=vector[i];
        vectorNou[i].producator = (char*) malloc(sizeof(char)* (strlen(vector[i].producator)+1));
        strcpy_s(vectorNou[i].producator, strlen(vector[i].producator)+1, vector[i].producator);
    }
	return vectorNou;
}

void dezalocare(struct Telefon *t) {
    if(t->producator != NULL){
        free(t->producator);
        t->producator=NULL;}
}

void copiazaAnumiteElemente(struct Telefon* vector, char nrElemente, float prag, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon t;
	t.id = 1;
    
	return t;
}





int main() {
	struct Telefon* telefoane=NULL;
	int nrTelefoane = 3;
    telefoane=(struct Telefon*)malloc(sizeof(struct Telefon)* nrTelefoane);
    telefoane[0] = initializare(1, 256, "Samsung", 2000, 'S');
    telefoane[1] = initializare(2, 512, "Motorola", 1500, 'M');
    telefoane[2] = initializare(3, 256, "Apple", 2200, 'A');
    afisareVector(telefoane, nrTelefoane);

    return 0;
}