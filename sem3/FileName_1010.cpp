#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996) //pentru a dezactiva _CRT_SECURE_NO_WARNINGS

struct Proprietar
{
	char* nume = NULL; //initializati tot timpul pointerii by default cu NULL
	int varsta;
};

struct Magazin
{
	char denumire[100];
	double venitLunar;
	int nrAngajati;
	char* celMaiVandutProdus = NULL; //initializati tot timpul pointerii by default cu NULL

	Proprietar proprietar;
};

void scrieProprietarInConsola(Proprietar p) {
	printf("Nume = %s\n", p.nume);
	printf("Varsta = %d\n", p.varsta);
}

void scrieProprietarInFisText(FILE* file, Proprietar p) {
	fprintf(file, "%s\n", p.nume);
	fprintf(file, "%d\n", p.varsta);
}

void citireProprietarDinFisText(FILE* file, Proprietar& p) {
	char aux[100];

	fgets(aux, 100, file);

	if (p.nume != NULL) free(p.nume);//evitare memory leaks

	p.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(p.nume, aux);

	fscanf(file, "%d\n", &p.varsta);
}

void scrieMagazinInConsola(Magazin m) {
	printf("Denumire = %s\n", m.denumire);

	printf("Venit lunar = %.2lf\n", m.venitLunar);
	printf("Nr angajati = %d\n", m.nrAngajati);
	printf("Cel mai vandut produs = %s\n", m.celMaiVandutProdus);

	scrieProprietarInConsola(m.proprietar);
}

void scrieMagazinInFisText(FILE* file, Magazin m) {
	fprintf(file, "%s\n", m.denumire);

	fprintf(file, "%.2lf\n", m.venitLunar);
	fprintf(file, "%d\n", m.nrAngajati);
	fprintf(file, "%s\n", m.celMaiVandutProdus);

	scrieProprietarInFisText(file, m.proprietar);
}

void citireMagazinDinFisText(FILE* file, Magazin& m) {
	char aux[100];

	fgets(aux, 100, file);
	strcpy(m.denumire, aux);

	fscanf(file, "%.2lf\n", &m.venitLunar);

	fscanf(file, "%d\n", &m.nrAngajati);

	fgets(aux, 100, file);

	if (m.celMaiVandutProdus != NULL) free(m.celMaiVandutProdus);//evitare memory leaks

	m.celMaiVandutProdus = (char*)malloc(strlen(aux) + 1);
	strcpy(m.celMaiVandutProdus, aux);

	citireProprietarDinFisText(file, m.proprietar);
}


void scrieMagazinInFisierBinar(FILE* file, Magazin m) {
	int dim = strlen(m.denumire) + 1;
	fwrite(&dim, sizeof(dim), 1, file);
	fwrite(m.denumire, dim, 1, file);

	fwrite(&m.venitLunar, sizeof(m.venitLunar), 1, file);
	fwrite(&m.nrAngajati, sizeof(m.nrAngajati), 1, file);

	dim = strlen(m.celMaiVandutProdus) + 1;
	fwrite(&dim, sizeof(dim), 1, file);
	fwrite(m.celMaiVandutProdus, dim, 1, file);

	//proprietar
	dim = strlen(m.proprietar.nume) + 1;
	fwrite(&dim, sizeof(dim), 1, file);
	fwrite(m.proprietar.nume, dim, 1, file);

	fwrite(&m.proprietar.varsta, sizeof(m.proprietar.varsta), 1, file);
}


void citireMagazinDinFisierBinar(FILE* file, Magazin& m) {
	int dim = 0;
	fread(&dim, sizeof(dim), 1, file);
	fread(m.denumire, dim, 1, file);

	fread(&m.venitLunar, sizeof(m.venitLunar), 1, file);
	fread(&m.nrAngajati, sizeof(m.nrAngajati), 1, file);

	if (m.celMaiVandutProdus != NULL) free(m.celMaiVandutProdus);

	char aux[100];

	fread(&dim, sizeof(dim), 1, file);
	fread(aux, dim, 1, file);

	m.celMaiVandutProdus = (char*)malloc(strlen(aux) + 1);
	strcpy(m.celMaiVandutProdus, aux);

	//proprietar
	if (m.proprietar.nume != NULL) free(m.proprietar.nume);

	fread(&dim, sizeof(dim), 1, file);
	fread(aux, dim, 1, file);

	m.proprietar.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(m.proprietar.nume, aux);

	fread(&m.proprietar.varsta, sizeof(m.proprietar.varsta), 1, file);
}

void main() {
	Proprietar p1, p2;

	p1.nume = (char*)malloc(strlen("Maria Popescu") + 1);
	strcpy(p1.nume, "Maria Popescu");
	p1.varsta = 31;

	p2.nume = (char*)malloc(strlen("Mihai Vasile") + 1);
	strcpy(p2.nume, "Mihai Vasile");
	p2.varsta = 27;

	Magazin m1, m2;

	strcpy(m1.denumire, "La Cocos");
	m1.venitLunar = 121122.4545;
	m1.nrAngajati = 20;
	m1.celMaiVandutProdus = (char*)malloc(strlen("Dero Color") + 1);
	strcpy(m1.celMaiVandutProdus, "Dero Color");
	m1.proprietar = p1;

	strcpy(m2.denumire, "Auchan Titan");
	m2.venitLunar = 2222772.4545;
	m2.nrAngajati = 31;
	m2.celMaiVandutProdus = (char*)malloc(strlen("Coca-Cola") + 1);
	strcpy(m2.celMaiVandutProdus, "Coca-Cola");
	m2.proprietar = p2;

	printf("\n\n---scriere in fisiere---\n\n");

	FILE* fisierMagazineOut = fopen("magazine.txt", "w");
	if (fisierMagazineOut == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru SCRIERE fisierul magazine.txt");
		exit(300);
	}

	scrieMagazinInFisText(fisierMagazineOut, m1);
	scrieMagazinInFisText(fisierMagazineOut, m2);

	fclose(fisierMagazineOut);



	FILE* fisierMagazineIn = fopen("magazine.txt", "r");
	if (fisierMagazineIn == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru CITIRE fisierul magazine.txt");
		exit(233);
	}

	Magazin m3, m4;
	citireMagazinDinFisText(fisierMagazineIn, m3);
	citireMagazinDinFisText(fisierMagazineIn, m4);


	fclose(fisierMagazineIn);

	printf("\n\n===afisare magazine din magazine.txt===\n\n");
	printf("\nm1\n");
	scrieMagazinInConsola(m1);
	printf("\nm2\n");
	scrieMagazinInConsola(m2);


	printf("\n\n===scriere si afisare - fisier binar===\n\n");
	FILE* fisierMagazineBinOut = fopen("magazine.bin", "wb");
	if (fisierMagazineBinOut == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru SCRIERE fisierul magazine.bin");
		exit(100);
	}

	scrieMagazinInFisierBinar(fisierMagazineBinOut, m1);
	scrieMagazinInFisierBinar(fisierMagazineBinOut, m2);

	fclose(fisierMagazineBinOut);


	FILE* fisierMagazineBinIn = fopen("magazine.bin", "rb");
	if (fisierMagazineBinIn == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru CITIRE fisierul magazine.bin");
		exit(120);
	}

	Magazin m5, m6;
	citireMagazinDinFisierBinar(fisierMagazineBinIn, m5);
	citireMagazinDinFisierBinar(fisierMagazineBinIn, m6);

	fclose(fisierMagazineBinIn);


	printf("\n\n===afisare magazine din magazine.bin===\n\n");
	printf("\nm5\n");
	scrieMagazinInConsola(m5);
	printf("\nm6\n");
	scrieMagazinInConsola(m6);
}