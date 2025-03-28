#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning (disable:4996)

struct data {
	int zi;
	int luna;
	int an;
};

//fisier binar relativ
struct angajat {
	int is; //is daca exista angajatul
	int cod; //is si cod reprezinta informatiile vitale la folosirea fis. binare relative

	char nume[50];
	char prenume[50];
	char dep[50];
	char prof[50];
	int varsta;
	float sal[3];

	data data_ang;
};

int nr_art(FILE* f)
{
	int l, m;

	l = ftell(f);//poz curenta
	fseek(f, 0, SEEK_END);//te duce la finalul fisierului

	m = ftell(f) / sizeof(angajat);

	fseek(f, l, SEEK_SET);//ma repozitionez de unde am plecat
	return m;
}

angajat createEmptyAngajat() {
	angajat a;

	a.is = 0;
	a.cod = 0;
	strcpy(a.nume, "EmptyNumeAngajat");
	strcpy(a.prenume, "EmptyPrenumeAngajat");
	strcpy(a.dep, "EmptyDepartamentAngajat");
	strcpy(a.prof, "EmptyProfesieAngajat");
	a.varsta = 0;

	//salariu pe 3 luni
	for (int i = 0; i < 3; i++) {
		a.sal[i] = 0;
	}

	a.data_ang.an = 0;
	a.data_ang.luna = 0;
	a.data_ang.zi = 0;

	return a;
}

void citireAng(angajat& a) {
	printf("nume="); getc(stdin); gets_s(a.nume);
	printf("prenume="); gets_s(a.prenume);
	printf("dep="); gets_s(a.dep);
	printf("prof="); gets_s(a.prof);
	printf("varsta="); scanf_s("%d", &a.varsta);
	for (int i = 0; i < 3; i++)
	{
		printf("sal din luna %d: ", i + 1);
		scanf_s("%f", &a.sal[i]);
	}
	printf("data angajarii zi, luna si an:  ");//03 05 2019
	scanf("%d %d %d", &a.data_ang.zi, &a.data_ang.luna, &a.data_ang.an);
}

void populare()
{
	FILE* f;

	angajat a = createEmptyAngajat();

	int cod, i, er;
	char nume_f[20];

	printf("\nnume fisier:");
	gets_s(nume_f);
	f = fopen(nume_f, "wb+");

	printf("cod="); scanf_s("%d", &cod);//reprezinta codul
	while (!feof(stdin))//<----sfarsitul introducerii fiind marcat standard - se citeste pana cand introduceti CTRL+Z
	{
		if (cod < 0) {
			continue;
		}

		//aici cod >= 0
		if (cod >= nr_art(f)) /// A1 A2 A3 A4 ..........A300 
		{
			fseek(f, 0, SEEK_END);

			for (i = nr_art(f); i < cod + 1; i++)
				fwrite(&a, sizeof(angajat), 1, f);

			a.is = 1;
			a.cod = cod;

			citireAng(a);

			fseek(f, cod * sizeof(angajat), SEEK_SET);
			fwrite(&a, sizeof(angajat), 1, f);
		}
		else {
			fseek(f, cod * sizeof(angajat), SEEK_SET);
			fread(&a, sizeof(angajat), 1, f);

			if (a.is == 0) {
				a.is = 1;
				a.cod = cod;

				citireAng(a);

				fseek(f, cod * sizeof(angajat), SEEK_SET);
				fwrite(&a, sizeof(angajat), 1, f);
			}
			else printf("inreg. exista\n");
		}
		printf("\ncod="); scanf_s("%d", &cod);
	}
	fclose(f);
}

void afisare()
{
	FILE* f, * g;
	angajat a = createEmptyAngajat();

	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);

	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\n===========================afisarea inregistrarilor din fisier=============");
		g = fopen("lista_fis.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "==============================================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || DEPARTAMENT || PROFESIE || VARSTA || LUNA 1 || LUNA 2 || LUNA 3 || DATA \n ");
		fprintf(g, "===================================================================================================================\n ");
		printf(" LISTA SALARII \n ");
		printf("=========================================================================================================================\n ");
		printf(" COD || NUME || PRENUME || DEPARTAMENT || PROFESIE || VARSTA || LUNA 1 || LUNA 2 || LUNA 3 || DATA \n ");
		printf("=========================================================================================================================\n ");

		fread(&a, sizeof(angajat), 1, f);
		while (!feof(f))//pana cand ajungi la sf fisierului
		{
			if (a.is == 1)
			{
				printf("\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f || %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
				fprintf(g, "\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f|| %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
			}

			fread(&a, sizeof(angajat), 1, f);
		}
		fclose(f); fclose(g);
	}
}

void afisare_poz()
{
	FILE* f, * g;
	angajat a = createEmptyAngajat();

	int n;
	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);
	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\nafisarea inregistrarilor a caror pozitie o citesc de la tastatura");
		g = fopen("lista_poz.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "=============================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || VARSTA || LUNA 1 ||LUNA 2 || LUNA 3\n ");
		fprintf(g, "==============================================================================================\n ");
		printf("\nn="); scanf("%d", &n);
		while (!feof(stdin))
		{
			if (n > nr_art(f)) printf("depaseste lungimea fis.");
			else
			{
				fseek(f, (n - 1) * sizeof(angajat), SEEK_SET);
				fread(&a, sizeof(angajat), 1, f);

				if (a.is == 1)
				{
					printf("\n%5d|| %-20s || %-20s ||%5d ||%5.2f||%5.2f||%5.2f", n, a.nume, a.prenume, a.varsta, a.sal[0], a.sal[1], a.sal[2]);
					fprintf(g, "\n%5d|| %-20s || %-20s ||%5d ||%5.2f||%5.2f||%5.2f", n, a.nume, a.prenume, a.varsta, a.sal[0], a.sal[1], a.sal[2]);
				}
				else printf("inregistrarea nu exista");
			}
			printf("\nn="); scanf("%d", &n);
		}
		fclose(f); fclose(g);
	}
}

void afisare_peBazaAnului()
{
	FILE* f, * g;
	angajat a = createEmptyAngajat();

	int an;
	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);
	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\nafisarea inregistrarilor pe baza anului");
		g = fopen("lista_angPeBazaAnului.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "=============================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || VARSTA || LUNA 1 ||LUNA 2 || LUNA 3\n ");
		fprintf(g, "==============================================================================================\n ");
		printf("\nan="); scanf("%d", &an);
		while (!feof(stdin))
		{
			fread(&a, sizeof(angajat), 1, f);
			while (!feof(f))//pana cand ajungi la sf fisierului
			{
				if (a.is == 1 && a.data_ang.an == an)
				{
					printf("\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f || %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
					fprintf(g, "\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f|| %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
				}
				fread(&a, sizeof(angajat), 1, f);
			}
			rewind(f);
			printf("\nan="); scanf("%d", &an);
		}
		fclose(f); fclose(g);
	}
}

void main() {
	//#define SEEK_CUR    1
	//#define SEEK_END    2
	//#define SEEK_SET    0

	printf("\n\POPULARE FISIER BINAR\n\n");
	populare();


	printf("\n\nAFISARE DIN FISIER BINAR\n\n");
	afisare();


	//printf("\n\AFISARE DIN FISIER BINAR PE BAZA CODULUI\n\n");
	//afisare_poz();
	printf("\n\AFISARE DIN FISIER BINAR PE BAZA ANULUI\n\n");
	afisare_peBazaAnului();
}