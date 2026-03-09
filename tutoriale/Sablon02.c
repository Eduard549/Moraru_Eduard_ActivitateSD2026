#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* dereferentierea la structuri se face cu -> de ex daca avem char* lista: ca sa accesam valoarea lui pret avem t->pret, daca lucram cu valoare obiect nu cu referinta, atunci folosim t.pret
   asignarea in functii a argumentelor se face prin valoare sau prin referinta!!!. daca e prin referinta, trebuie sa dereferentiem ca sa accesam/modificam valorile,
   daca asignam doar valori, se fac copii ale argumentelor noastre in parametrii.  
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

void dezalocare(struct Telefon **vector, int* nrElemente) {
    for(int i = 0; i<(*nrElemente); i++){
        if((*vector)[i].producator!=NULL){
        free((*vector)[i].producator);
        }
    }
    free(*vector);
    *vector=NULL;
    *nrElemente=0;
    // if(t->producator != NULL){
    //     free(t->producator);
    //     t->producator=NULL;}
}

void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {
    *dimensiune = 0;
    for(int i=0; i<nrElemente;i++){
        if(vector[i].pret >= pretMinim){
            (*dimensiune)++;
        }
    }
    if((*vectorNou) != NULL){
        free(*vectorNou);
    }
    *vectorNou=(struct Telefon*)malloc(sizeof(struct Telefon)*(*dimensiune));
    int k=0;
    for(int i=0;i<nrElemente;i++){
        if(vector[i].pret >= pretMinim){
            (*vectorNou)[k]=vector[i];
            (*vectorNou)[k].producator = (char*) malloc(sizeof(char)* (strlen(vector[i].producator)+1));
            strcpy_s((*vectorNou)[k].producator, strlen(vector[i].producator)+1, vector[i].producator);
            k++;

        }
    }
}

struct Telefon getPrimulTelefonProducator(struct Telefon* vector, int nrElemente, const char* producator) {

	struct Telefon t;
    for(int i=0; i< nrElemente; i++){
        if(strcmp(vector[i].producator, producator)==0){
            t= vector[i];
            t.producator=(char*)malloc(strlen(vector[i].producator)+1);
            strcpy_s(t.producator, strlen(vector[i].producator)+1, vector[i].producator);
            return t;
        }
    }
    
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

    struct Telefon* primeleTelefoane = NULL;
    int nrPrimeleTelefoane = 2;
    primeleTelefoane = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrPrimeleTelefoane);
    printf("\n\nPrimele %d telefoane: \n", nrPrimeleTelefoane);
    afisareVector(primeleTelefoane, nrPrimeleTelefoane);
    dezalocare(&primeleTelefoane, &nrPrimeleTelefoane);
    struct Telefon* telefoaneScumpe = NULL;
    int nrTelefoaneScumpe = 0;
    copiazaTelefoaneScumpe(telefoane, nrTelefoane,2500, &telefoaneScumpe, &nrTelefoaneScumpe);
    printf("\n\nTelefoane scumpe: \n");
    afisareVector(telefoaneScumpe, nrTelefoaneScumpe);
    dezalocare(&telefoaneScumpe, &nrTelefoaneScumpe);

    struct Telefon telefon=getPrimulTelefonProducator(telefoane, nrTelefoane, "Samsung");
    printf("\nTelefonul gasit: \n");
    afisare(telefon);
    if(telefon.producator != NULL){
        free(telefon.producator);
        telefon.producator=NULL;
    }

    dezalocare(&telefoane, &nrTelefoane);
    return 0;
}