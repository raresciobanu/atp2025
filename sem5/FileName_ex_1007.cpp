#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#pragma warning (disable:4996)

//fisier binar secvential
struct student
{
	int nrAbsente;
	char nume[100];
	char serie[30];
	double medieAnPrecedent;
	char telefon[20];
};

void citireStudent(student& s) {
	printf("\n========\n");

	printf("nume = "); gets_s(s.nume, 100);

	printf("nrAbsente = "); scanf("%d", &s.nrAbsente);

	printf("medieAnPrecedent = "); scanf("%lf", &s.medieAnPrecedent);

	getchar();
	printf("serie = "); gets_s(s.serie, 30);

	printf("telefon = "); gets_s(s.telefon, 20);

	printf("\n========\n");
}

void populare() {
	FILE* f = fopen("student.bin", "wb");

	if (f == NULL) {
		printf("Eroare deschidere fisier.\n");
		return;
	}
	else {
		student s;

		int continuare = 1;
		while (continuare == 1) {
			citireStudent(s);

			fwrite(&s, sizeof(student), 1, f);

			printf("Doriti sa continuati?(1 - DA; 0 - NU)\n");
			scanf("%d", &continuare);
			getchar();//ramane \n de la citirea lui  continuare 
		}

		fclose(f);
	}
}

void scrieInFisText(student s, FILE* f) {
	fprintf(f, "%s\n", s.nume);
	fprintf(f, "%d\n", s.nrAbsente);
	fprintf(f, "%s\n", s.serie);
	fprintf(f, "%.2lf\n", s.medieAnPrecedent);
	fprintf(f, "%s\n", s.telefon);
}

void citireDinFisBinarSiAfisareInFisTxt() {
	FILE* fisBin = fopen("student.bin", "rb");
	FILE* fisTxt = fopen("student.txt", "w");

	if (fisBin == NULL || fisTxt == NULL) {
		printf("nu s-au putut deschide fisirele\n");
	}
	else {
		student s;
		while (fread(&s, sizeof(student), 1, fisBin) == 1) {
			scrieInFisText(s, fisTxt);
		}

		fclose(fisBin);
		fclose(fisTxt);
	}
}

void incrementeazaMedieStudent() {
	FILE* fisBin = fopen("student.bin", "rb+");

	if (fisBin == NULL) {
		printf("nu s-a putut deschide fisirul\n");
	}
	else {
		char numeStudent[100];

		printf("nume student = "); gets_s(numeStudent, 100);

		student s;
		int pozitie = 0;
		while (fread(&s, sizeof(student), 1, fisBin) == 1) {
			if (strcmp(s.nume, numeStudent) == 0) {
				if (s.medieAnPrecedent <= 5) {
					s.medieAnPrecedent++;

					fseek(fisBin, pozitie * sizeof(student), SEEK_SET);
					fwrite(&s, sizeof(student), 1, fisBin);

					printf("media a crescut cu un punct pentru %s\n", numeStudent);
					break;
				}
			}

			pozitie++;
		}

		fclose(fisBin);
	}
}

void raportRestantieri() {
	FILE* fisBin = fopen("student.bin", "rb+");
	FILE* fisTxt = fopen("restantieri.txt", "w");

	if (fisBin == NULL || fisTxt == NULL) {
		printf("nu s-au putut deschide fisirele\n");
	}
	else {
		student s;
		while (fread(&s, sizeof(student), 1, fisBin) == 1) {
			if (s.medieAnPrecedent < 5) {
				fprintf(fisTxt, "%s => %.2lf\n", s.nume, s.medieAnPrecedent);
			}
		}

		fclose(fisTxt);
		fclose(fisBin);
	}
}
void main() {

	printf("\n\n==populare==\n\n");
	populare();

	printf("\n\n==citireDinFisBinarSiAfisareInFisTxt==\n\n");
	citireDinFisBinarSiAfisareInFisTxt();

	printf("\n\n==incrementeazaMedieStudent==\n\n");
	incrementeazaMedieStudent();

	printf("\n\n==citireDinFisBinarSiAfisareInFisTxt==\n\n");
	citireDinFisBinarSiAfisareInFisTxt();

	printf("\n\n==raportRestantieri==\n\n");
	raportRestantieri();
}