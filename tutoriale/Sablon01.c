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

void modificaPret(struct Telefon* t, float pretNou) {
	if(pretNou > 0){t->pret = pretNou;}
}

void dezalocare(struct Telefon *t) {
    if(t->producator != NULL){
        free(t->producator);
        t->producator=NULL;}
}

int main() {
	struct Telefon t;
    t= initializare(1, 256, "Samsung", 2000.5, 'A');
    modificaPret(&t, 1000);
    afisare(t);
    dezalocare(&t);
	return 0;
}