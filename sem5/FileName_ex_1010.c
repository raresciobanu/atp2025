#include<stdio.h>
#include<string.h>

#pragma warning (disable:4996)

//fisier binar relativ
typedef struct {
	int esteSetat; //0 sau 1
	int key;

	char denumire[100];
	double venitLunar;
	int nrAngajati;
} magazin;

int nr_art(FILE* f)
{
	int l, m;

	l = ftell(f);//poz curenta
	fseek(f, 0, SEEK_END);//te duce la finalul fisierului

	m = ftell(f) / sizeof(magazin);

	fseek(f, l, SEEK_SET);//ma repozitionez de unde am plecat
	return m;
}

magazin genereazaMagazinGol() {
	magazin m;

	m.esteSetat = 0;
	m.nrAngajati = 0;
	m.venitLunar = 0.0;
	m.key = -1;//nu exista cheia
	strcpy(m.denumire, "emptyName");

	return m;
}

void citireMagazin(magazin* m) {
	m->esteSetat = 1;

	printf("nrAngajati = "); scanf("%d", &m->nrAngajati);
	printf("venitLunar = "); scanf("%lf", &m->venitLunar);

	getchar();
	printf("denumire = "); fgets(m->denumire, 100, stdin);
}

void populare() {
	magazin magazinEmpty = genereazaMagazinGol();

	FILE* f = fopen("magazine.bin", "wb+");

	int cod;
	printf("cod="); scanf("%d", &cod);//reprezinta codul>=0
	while (!feof(stdin))// se citeste pana cand introduceti CTRL+Z
	{
		if (cod < 0) {
			printf("cod trebuie sa fie >=0\n");
			continue;
		}

		int nrMagazine = nr_art(f);
		//trebuie sa aloci mai multa memorie in fisier
		if (cod > nrMagazine)
		{
			fseek(f, 0, SEEK_END);

			for (int i = 0; i <= cod - nrMagazine; i++) {
				fwrite(&magazinEmpty, sizeof(magazin), 1, f);
			}

			fseek(f, cod * sizeof(magazin), SEEK_SET);

			magazin temp;
			temp.key = cod;

			citireMagazin(&temp);

			fwrite(&temp, sizeof(magazin), 1, f);
		}
		else {
			fseek(f, cod * sizeof(magazin), SEEK_SET);

			magazin temp;
			fread(&temp, sizeof(magazin), 1, f);

			if (temp.esteSetat == 1) {
				printf("nu poti suprascrie pozitia %d\n", cod);
			}
			else {
				fseek(f, cod * sizeof(magazin), SEEK_SET);

				citireMagazin(&temp);
				temp.key = cod;

				fwrite(&temp, sizeof(magazin), 1, f);
			}
		}

		printf("\n\ncod="); scanf("%d", &cod);
	}

	fclose(f);
}

void scrieInFisText(magazin m, FILE* fisText) {
	fprintf(fisText, "%d\n", m.key);
	fprintf(fisText, "%d\n", m.nrAngajati);
	fprintf(fisText, "%s\n", m.denumire);
	fprintf(fisText, "%.2lf\n", m.venitLunar);
}

void scrieInFisTextMagazineleCu5Angajati() {
	FILE* fisBin = fopen("magazine.bin", "rb");
	FILE* fisText = fopen("magazineCu5Angajati.txt", "w");

	int nrMagazine = nr_art(fisBin);

	magazin temp;
	for (int i = 0; i < nrMagazine; i++) {
		fread(&temp, sizeof(magazin), 1, fisBin);
		if (temp.esteSetat == 1 && temp.nrAngajati >= 5) {
			scrieInFisText(temp, fisText);
		}
	}

	fclose(fisBin);
	fclose(fisText);
}


int main() {
	//#define SEEK_CUR    1
	//#define SEEK_END    2
	//#define SEEK_SET    0


	printf("\n\n==populare==\n\n");
	populare();

	printf("\n\n==scrieInFisTextMagazineleCu5Angajati==\n\n");
	scrieInFisTextMagazineleCu5Angajati();

	return 0;
}
