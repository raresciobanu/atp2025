#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#pragma warning(disable:4996) //pentru a dezactiva _CRT_SECURE_NO_WARNINGS

enum CULORI
{
	ALB,
	NEGRU = 10,
	VERDE = 20,
	ROSU,
};

// const int * --> ce se afla la zona de memorie catre care pointeaza pointerul este constanta.
// int * const --> var pointer este constanta (nu poti altera adresa catre care pointeaza pointerul).
// const int * const -> un pointer constant ce pointeaza catre o zona de memorie constanta

const char* transformaEnumInSirDeCaractere(enum CULORI culoare) {
	switch (culoare)
	{
	case ALB:
		return "alb";
	case NEGRU:
		return "negru";
	case VERDE:
		return "verde";
	case ROSU:
		return "rosu";
	default:
		return "culoarea introdusa nu a fost interpretata";
	}
}

enum CULORI transformaSirDeCaractereInEnum(const char* pointerSir) {
	if (strcmp(pointerSir, "alb") == 0) {
		return ALB;
	}
	else if (strcmp(pointerSir, "negru") == 0) {
		return NEGRU;
	}
	else if (strcmp(pointerSir, "verde") == 0) {
		return VERDE;
	}
	else if (strcmp(pointerSir, "rosu") == 0) {
		return ROSU;
	}
}

//104 bytes
struct client {
	char nume[100];//100 bytes
	int varsta;//4 bytes
};

//100 bytes
union user {
	int varsta; //4bytes
	char nume[100]; //100 bytes
	char gen; //1 bytes
};

//129bytes
struct telefon
{
	enum CULORI culoareTelefon;//4bytes
	char* marca;//8bytes
	double pret;//8bytes
	int anFabricatie;//4bytes
	char version;//1byte
	struct client proprietar;//104bytes
};

void scrieClientLaConsola(struct client c) {
	printf("varsta = %d\n", c.varsta);

	printf("nume = %s\n\n", c.nume);
}

void citesteClientDeLaConsola(struct client* c) {
	printf("varsta = ");
	scanf("%d", &c->varsta);


	printf("client = ");
	getchar();// folosit pt a elibera bufferul
	fgets(c->nume, 99, stdin);
	//scanf("%s", c.nume); //scanf citeste pana la primul spatiu
}

void scrieClientInFisier(FILE* fisier, struct client c) {
	fprintf(fisier, "%d\n", c.varsta);
	fprintf(fisier, "%s\n", c.nume);
}

void citireClientDinFisier(FILE* fisier, struct client* c) {
	fscanf(fisier, "%d\n", &c->varsta);
	fgets(c->nume, 100, fisier);
}

void scrieTelefonLaConsola(struct telefon t) {
	printf("culoare telefon = %s\n", transformaEnumInSirDeCaractere(t.culoareTelefon));

	printf("marca = %s\n", t.marca);
	printf("pret = %.2lf\n", t.pret);
	printf("an fabricatie = %d\n", t.anFabricatie);
	printf("version = %c\n", t.version);

	scrieClientLaConsola(t.proprietar);
	//linia de mai sus este echivalenta cu:
	//printf("client = %s\n", t.proprietar.nume);
	//printf("varsta = %d\n\n", t.proprietar.varsta);
}

void citireTelefonDeLaConsola(struct telefon* t) {
	char aux[100];
	printf("culoare telefon (alb / negru / verde / rosu) = ");
	scanf("%s", aux);

	t->culoareTelefon = transformaSirDeCaractereInEnum(aux);

	printf("marca = ");
	scanf("%s", aux);

	if (t->marca != NULL) {
		free(t->marca);//evitare memory leaks
	}

	t->marca = (char*)malloc(strlen(aux) + 1);//+1 pt \0
	strcpy(t->marca, aux);


	printf("pret = ");
	scanf("%lf", &t->pret);

	printf("an fabricatie = ");
	scanf("%d", &t->anFabricatie);

	printf("version = ");
	scanf(" %c", &t->version);//daca tu introduci de la tastatura 44 (in spate inseamna 44\n)=> automat t.version=\n
	//ca sa evitam aceasta problema (sa ignore un \n citit inainte) punem un spatiu in fata %c

	//getchar();// folosit pt a elibera bufferul....cand citesti versiunea ( se citeste defapt de ex: M\n) => ramane in buffer \n

	citesteClientDeLaConsola(&t->proprietar);
}

void scrieTelefonInFisier(FILE* fisier, struct telefon t) {
	fprintf(fisier, "%s\n", transformaEnumInSirDeCaractere(t.culoareTelefon));

	fprintf(fisier, "%s\n", t.marca);
	fprintf(fisier, "%.2lf\n", t.pret);
	fprintf(fisier, "%d\n", t.anFabricatie);
	fprintf(fisier, "%c\n", t.version);

	scrieClientInFisier(fisier, t.proprietar);
}

void citireTelefonDinFisier(FILE* fisier, struct telefon* t) {
	char aux[100];

	fscanf(fisier, "%s", aux);
	t->culoareTelefon = transformaSirDeCaractereInEnum(aux);

	fscanf(fisier, "%s", aux);

	if (t->marca != NULL) {
		free(t->marca);//evitare memory leaks
	}

	t->marca = (char*)malloc(strlen(aux) + 1);//+1 pt \0
	strcpy(t->marca, aux);


	fscanf(fisier, "%lf", &t->pret);

	fscanf(fisier, "%d", &t->anFabricatie);

	fscanf(fisier, " %c", &t->version);//daca tu introduci de la tastatura 44 (in spate inseamna 44\n)=> automat t.version=\n
	//ca sa evitam aceasta problema (sa ignore un \n citit inainte) punem un spatiu in fata %c

	//fgetc(fisier);// folosit pt a elibera bufferul....cand citesti versiunea ( se citeste defapt de ex: M\n) => ramane in buffer \n

	citireClientDinFisier(fisier, &t->proprietar);
}

void scrieClientInFisierBinar(FILE* fisier, struct client c) {
	fwrite(&c.varsta, sizeof(c.varsta), 1, fisier);

	int dim = strlen(c.nume) + 1;//calculam dim numelui clientului (+1 pt \0)
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(c.nume, dim, 1, fisier);

	//alternativ putem face ca mai jos daca structura noastra nu contine pointeri
	//dar daca scriem asa...nu suntem eficienti...blocam tot timpul 104bytes(dim clientului)
	//fwrite(&c, sizeof(c), 1, fisier);
}

void citireClientDinFisierBinar(FILE* fisier, struct client* c) {
	fread(&c->varsta, sizeof(c->varsta), 1, fisier);

	int dim = 0;
	fread(&dim, sizeof(dim), 1, fisier);
	fread(c->nume, dim, 1, fisier);

	//alternativ putem face ca mai jos daca structura noastra nu contine pointeri
	//dar daca scriem asa...nu suntem eficienti...blocam tot timpul 104bytes(dim clientului)
	//fread(&c, sizeof(c), 1, fisier);
}

void scrieTelefonInFisierBinar(FILE* fisier, struct telefon t) {
	//functia fwrite - primeste ca input ===> fwrite(addressData, sizeData, numbersData, pointerToFile);
	//addressData - adresa variabilei citite (cu "&" in fata)
	//sizeData - dimensiunea variabilei salvate (sizeof)
	//numbersData - numarul de variabile salvate
			//int arr[3] = {101, 203, 303};
			//
			//fwrite(arr, sizeof(int), 2, fp); //va scrie primele 2 elemente din vector
	//pointerToFile - fisierul unde se salveaza variabila

	//DACA scrierea are erori atunci fwrite va retuna un numar mai mic decat variabila numbersData;

	//fwrite(&t, sizeof(l), 1, fisier); //Daca clientul nu ar fi avut char* puteam sa scriem direct asa.

	char aux[100];
	strcpy(aux, transformaEnumInSirDeCaractere(t.culoareTelefon));

	int dim = strlen(aux) + 1;//calculam dim culorii telefonului (+1 pt \0)
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(aux, dim, 1, fisier);

	dim = strlen(t.marca) + 1;//calculam dim marcii telefonului (+1 pt \0)
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(t.marca, dim, 1, fisier);

	fwrite(&t.pret, sizeof(t.pret), 1, fisier);

	fwrite(&t.anFabricatie, sizeof(t.anFabricatie), 1, fisier);

	fwrite(&t.version, sizeof(t.version), 1, fisier);

	scrieClientInFisierBinar(fisier, t.proprietar);
}

void citireTelefonDinFisierBinar(FILE* fisier, struct telefon* t) {
	//functia fread - primeste ca input===> fread(addressData, sizeData, numbersData, pointerToFile);
	//descrierea parametrilor este la fel ca la fwrite

	//fread(&l, sizeof(l), 1, fisier); //daca nu aveam char* in structura client scriam asa.
	int dim = 0;
	fread(&dim, sizeof(dim), 1, fisier);
	char aux[100];
	fread(aux, dim, 1, fisier);
	t->culoareTelefon = transformaSirDeCaractereInEnum(aux);

	if (t->marca != NULL) {
		free(t->marca);//evitare memory leaks
	}

	fread(&dim, sizeof(dim), 1, fisier);
	fread(aux, dim, 1, fisier);
	t->marca = (char*)malloc(strlen(aux) + 1);//+1 pt \0
	strcpy(t->marca, aux);

	fread(&t->pret, sizeof(t->pret), 1, fisier);

	fread(&t->anFabricatie, sizeof(t->anFabricatie), 1, fisier);

	fread(&t->version, sizeof(t->version), 1, fisier);

	citireClientDinFisierBinar(fisier, &t->proprietar);
}

int main() {
	//testare adresa vector
	char nume[100];
	printf("nume=%p\n", nume);
	printf("&nume=%p\n", &nume);
	printf("&nume[0]=%p\n", &nume[0]);
	printf("&nume[1]=%p\n\n\n", &nume[1]);


	union user user1;
	user1.varsta = 20;
	printf("varsta=%d\n", user1.varsta);
	user1.gen = 'M';
	printf("gen=%c\n", user1.gen);
	printf("varsta=%d\n", user1.varsta);
	printf("varsta=%c\n", user1.varsta);
	printf("varsta=%p\n", user1.varsta);

	printf("\n\n---scriere in fisiere---\n\n");
	// scriere in fisiere
	//w-creare fisier si scriere in el, daca exista fisierul exista se sterg toate informatiile din el
	//wb-este "w" in mod binar
	//r-citire din fisier, daca fisierul nu exista se intoarce NULL la fopen()
	//rb-este "r" in mod binar, daca fisierul nu exista se intoarce NULL la fopen()
	//a-adaugare informatii la sf fisierului, daca NU exista fisierul se face unul nou
	//ab- este "a" in mod binar
	//r+ sau rb+ - fisier deschis pentru citire si scriere/ citire si scriere in mod binar, daca fisierul nu exista se intoarce NULL la fopen()
	//w+ sau wb+ - fisier deschis pentru citire si scriere in mod binar.
	//a+ si ab+ - fisier deschis pentru citire si adaugare la final


	FILE* fisierClienti = fopen("clienti.txt", "w");
	if (fisierClienti == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru scriere fisierul clienti.txt");
		exit(300);
	}

	struct client c1;
	strcpy(c1.nume, "Rares");
	c1.varsta = 20;

	struct client c2;
	strcpy(c2.nume, "Ana");
	c2.varsta = 21;
	struct client c3;
	citesteClientDeLaConsola(&c3);

	printf("\n\n---scriere CLIENT la consola---\n\n");

	scrieClientLaConsola(c1);
	scrieClientLaConsola(c2);
	scrieClientLaConsola(c3);

	printf("\n\n---scrierea a 3 CLIENTI in fisier text---\n\n");

	scrieClientInFisier(fisierClienti, c1);
	scrieClientInFisier(fisierClienti, c2);
	scrieClientInFisier(fisierClienti, c3);
	fclose(fisierClienti);


	printf("\n\n---citirea a 3 CLIENTI din fisier text---\n\n");
	FILE* fisierClientiReadMode = fopen("clienti.txt", "r");
	if (fisierClientiReadMode == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru citire fisierul clienti.txt");
		exit(900);
	}

	struct client c4, c5, c6;
	citireClientDinFisier(fisierClientiReadMode, &c4);
	citireClientDinFisier(fisierClientiReadMode, &c5);
	citireClientDinFisier(fisierClientiReadMode, &c6);

	//important! inchideti fisierul dupa ce terminati de lucrat in el
	fclose(fisierClientiReadMode);


	printf("\n\n---scriere CLIENT (c4,c5,c6) din fisier text la consola---\n\n");

	scrieClientLaConsola(c4);
	scrieClientLaConsola(c5);
	scrieClientLaConsola(c6);



	printf("\n\n---citire TELEFON DE LA consola---\n\n");
	struct telefon t1, t2, t3;
	t1.anFabricatie = 2020;
	t2.anFabricatie = 2021;

	t1.culoareTelefon = NEGRU;
	t2.culoareTelefon = VERDE;

	t1.pret = 2000.99;
	t2.pret = 2599.99;

	t1.version = '6';
	t2.version = 'X';

	t1.proprietar = c1;
	t2.proprietar = c2;

	//pt a declara un sir de caractere pe stack ai face asa:
	//char aux[100]; //cand scrii asa de fapt variabila aux reprezinta adresa primului element din vector adica aux[0];
	//strcpy(aux, "ceva");

	//pt a declara un sir de caractere pe heap ai face asa:
	//1) rezervi intai spatiu in memorie
	t1.marca = (char*)malloc(strlen("Apple") + 1);//se aloca memorie pe heap pt 6 bytes (apple =  5 si +1=6 pentru \0);
	//2) copiezi informatia la acea zona de memorie
	strcpy(t1.marca, "Apple");


	t2.marca = (char*)malloc(strlen("Huawei") + 1);
	strcpy(t2.marca, "Huawei");


	citireTelefonDeLaConsola(&t3);

	printf("\n\n---scriere TELEFON la consola---\n\n");

	scrieTelefonLaConsola(t1);
	scrieTelefonLaConsola(t2);
	scrieTelefonLaConsola(t3);


	printf("\n\n---scriere TELEFOANE in fisier text---\n\n");
	FILE* fisierTelefoane = fopen("telefoane.txt", "w");
	if (fisierTelefoane == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru scriere fisierul telefoane.txt");
		exit(1000);
	}

	scrieTelefonInFisier(fisierTelefoane, t1);
	scrieTelefonInFisier(fisierTelefoane, t2);
	scrieTelefonInFisier(fisierTelefoane, t3);

	fclose(fisierTelefoane);


	printf("\n\n---citire TELEFOANE din fisier text---\n\n");
	fisierTelefoane = fopen("telefoane.txt", "r");
	if (fisierTelefoane == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru citire fisierul telefoane.txt");
		exit(2000);
	}

	struct telefon t4, t5, t6;
	citireTelefonDinFisier(fisierTelefoane, &t4);
	citireTelefonDinFisier(fisierTelefoane, &t5);
	citireTelefonDinFisier(fisierTelefoane, &t6);

	scrieTelefonLaConsola(t4);
	scrieTelefonLaConsola(t5);
	scrieTelefonLaConsola(t6);



	//fisire BINARE
	printf("\n\n---scriere TELEFOANE in fisier binar---\n\n");
	FILE* fisierTelefoaneBinar = fopen("telefoaneBinar.bin", "wb");
	if (fisierTelefoaneBinar == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru scriere fisierul binar telefoaneBinar.bin");
		exit(5500);
	}

	scrieTelefonInFisierBinar(fisierTelefoaneBinar, t1);
	scrieTelefonInFisierBinar(fisierTelefoaneBinar, t2);

	fclose(fisierTelefoaneBinar);


	printf("\n\n---citire TELEFOANE din fisier binar---\n\n");
	fisierTelefoaneBinar = fopen("telefoaneBinar.bin", "rb");
	if (fisierTelefoaneBinar == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru citire fisierul binar telefoaneBinar.bin");
		exit(7700);
	}

	struct telefon t10, t11;
	citireTelefonDinFisierBinar(fisierTelefoaneBinar, &t10);
	citireTelefonDinFisierBinar(fisierTelefoaneBinar, &t11);

	scrieTelefonLaConsola(t10);
	scrieTelefonLaConsola(t11);

	fclose(fisierTelefoaneBinar);


	free(t1.marca);
	free(t2.marca);
	free(t3.marca);
	free(t4.marca);
	free(t5.marca);
	free(t6.marca);
	free(t10.marca);
	free(t11.marca);
}
