#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996) //pentru a dezactiva _CRT_SECURE_NO_WARNINGS

struct DataSchimbareUlei {
	int zi;
	int luna;
	int an;
};

struct Marca
{
	char* denumire = NULL; //initializati tot timpul pointerii by default cu NULL
	int nrModele;
};

struct Masina
{
	double nrKm;
	int anFabricatie;
	char* proprietar = NULL; //initializati tot timpul pointerii by default cu NULL

	DataSchimbareUlei dataSchimbareUlei;
	Marca marca;
};

void scrieLaConsola(Masina m) {
	printf("Nr. km = %.2lf\n", m.nrKm);
	printf("An fabricatie = %d\n", m.anFabricatie);
	printf("Proprietar = %s\n", m.proprietar);

	//DataSchimbareUlei
	printf("Zi = %d\n", m.dataSchimbareUlei.zi);
	printf("Luna = %d\n", m.dataSchimbareUlei.luna);
	printf("An = %d\n", m.dataSchimbareUlei.an);

	//Marca
	printf("Denumire = %s\n", m.marca.denumire);
	printf("Nr modele = %d\n", m.marca.nrModele);
}

void scrieInFisierText(FILE* file, Masina m) {
	fprintf(file, "%.2lf\n", m.nrKm);
	fprintf(file, "%d\n", m.anFabricatie);
	fprintf(file, "%s\n", m.proprietar);

	//DataSchimbareUlei
	fprintf(file, "%d\n", m.dataSchimbareUlei.zi);
	fprintf(file, "%d\n", m.dataSchimbareUlei.luna);
	fprintf(file, "%d\n", m.dataSchimbareUlei.an);

	//Marca
	fprintf(file, "%s\n", m.marca.denumire);
	fprintf(file, "%d\n", m.marca.nrModele);
}

void citireDinFisierText(FILE* file, Masina& m) {
	fscanf(file, "%lf", &m.nrKm);
	fscanf(file, "%d", &m.anFabricatie);

	if (m.proprietar != NULL) free(m.proprietar);//evitare memory leaks

	fgetc(file);

	char aux[100];
	fgets(aux, 100, file);
	m.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy(m.proprietar, aux);

	//DataSchimbareUlei
	fscanf(file, "%d", &m.dataSchimbareUlei.zi);
	fscanf(file, "%d", &m.dataSchimbareUlei.luna);
	fscanf(file, "%d", &m.dataSchimbareUlei.an);

	//Marca
	fgetc(file);

	if (m.marca.denumire != NULL) free(m.marca.denumire);//evitare memory leaks

	fgets(aux, 100, file);
	m.marca.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy(m.marca.denumire, aux);

	fscanf(file, "%d", &m.marca.nrModele);
}

void scrieInFisBinar(FILE* file, Masina m) {
	fwrite(&m.nrKm, sizeof(m.nrKm), 1, file);
	fwrite(&m.anFabricatie, sizeof(m.anFabricatie), 1, file);

	int dim = strlen(m.proprietar) + 1;
	fwrite(&dim, sizeof(dim), 1, file);
	fwrite(m.proprietar, dim, 1, file);

	fwrite(&m.dataSchimbareUlei.zi, sizeof(m.dataSchimbareUlei.zi), 1, file);
	fwrite(&m.dataSchimbareUlei.luna, sizeof(m.dataSchimbareUlei.luna), 1, file);
	fwrite(&m.dataSchimbareUlei.an, sizeof(m.dataSchimbareUlei.an), 1, file);

	dim = strlen(m.marca.denumire) + 1;
	fwrite(&dim, sizeof(dim), 1, file);
	fwrite(m.marca.denumire, dim, 1, file);

	fwrite(&m.marca.nrModele, sizeof(m.marca.nrModele), 1, file);
}

void citireDinFisBinar(FILE* file, Masina& m) {
	fread(&m.nrKm, sizeof(m.nrKm), 1, file);
	fread(&m.anFabricatie, sizeof(m.anFabricatie), 1, file);

	if (m.proprietar != NULL) free(m.proprietar);//evitare memory leaks

	int dim = 0;
	char aux[100];

	fread(&dim, sizeof(dim), 1, file);
	fread(aux, dim, 1, file);

	m.proprietar = (char*)malloc(strlen(aux) + 1);
	strcpy(m.proprietar, aux);

	fread(&m.dataSchimbareUlei.zi, sizeof(m.dataSchimbareUlei.zi), 1, file);
	fread(&m.dataSchimbareUlei.luna, sizeof(m.dataSchimbareUlei.luna), 1, file);
	fread(&m.dataSchimbareUlei.an, sizeof(m.dataSchimbareUlei.an), 1, file);

	if (m.marca.denumire != NULL) free(m.marca.denumire);//evitare memory leaks

	fread(&dim, sizeof(dim), 1, file);
	fread(aux, dim, 1, file);

	m.marca.denumire = (char*)malloc(strlen(aux) + 1);
	strcpy(m.marca.denumire, aux);

	fread(&m.marca.nrModele, sizeof(m.marca.nrModele), 1, file);
}

void main() {
	DataSchimbareUlei d1, d2;
	d1.zi = 22; d1.luna = 6; d1.an = 2024;
	d2.zi = 10; d2.luna = 12; d2.an = 2024;

	Marca marca1, marca2;
	marca1.denumire = (char*)malloc(strlen("Audi") + 1);
	strcpy(marca1.denumire, "Audi");
	marca1.nrModele = 120;

	marca2.denumire = (char*)malloc(strlen("BMW") + 1);
	strcpy(marca2.denumire, "BMW");
	marca2.nrModele = 170;

	Masina m1, m2;

	m1.anFabricatie = 2020;
	m1.nrKm = 200010.993;
	m1.proprietar = (char*)malloc(strlen("Popescu Maria") + 1);
	strcpy(m1.proprietar, "Popescu Maria");
	m1.dataSchimbareUlei = d1;
	m1.marca = marca1;

	m2.anFabricatie = 2024;
	m2.nrKm = 155000.10;
	m2.proprietar = (char*)malloc(strlen("Ionescu Vasile") + 1);
	strcpy(m2.proprietar, "Ionescu Vasile");
	m2.dataSchimbareUlei = d2;
	m2.marca = marca2;

	FILE* fisTxtOut = fopen("masini.txt", "w");
	if (fisTxtOut == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru SCRIERE fisierul masini.txt");
		exit(300);
	}

	scrieInFisierText(fisTxtOut, m1);
	scrieInFisierText(fisTxtOut, m2);

	fclose(fisTxtOut);


	FILE* fisTxtIn = fopen("masini.txt", "r");
	if (fisTxtIn == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru CITIRE fisierul masini.txt");
		exit(200);
	}

	Masina m3, m4;
	citireDinFisierText(fisTxtIn, m3);
	citireDinFisierText(fisTxtIn, m4);

	fclose(fisTxtIn);

	printf("\nm3\n");
	scrieLaConsola(m3);
	printf("\nm4\n");
	scrieLaConsola(m4);

	printf("\n\n-----fis binare-----\n\n");

	FILE* fisBinOut = fopen("masini.bin", "wb");
	if (fisBinOut == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru SCRIERE fisierul masini.bin");
		exit(100);
	}

	scrieInFisBinar(fisBinOut, m1);
	scrieInFisBinar(fisBinOut, m2);

	fclose(fisBinOut);


	FILE* fisBinIn = fopen("masini.bin", "rb");
	if (fisBinIn == NULL) {
		printf("EROARE: Nu s-a putut deschide pentru CITIRE fisierul masini.bin");
		exit(600);
	}

	Masina m5, m6;
	citireDinFisBinar(fisBinIn, m5);
	citireDinFisBinar(fisBinIn, m6);

	fclose(fisBinIn);

	printf("\nm5\n");
	scrieLaConsola(m5);
	printf("\nm6\n");
	scrieLaConsola(m6);
}