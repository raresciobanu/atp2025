#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//fisier binar secvential
typedef struct
{
	int zi;
	int luna;
	int an;
} Data;

typedef struct {
	int codDeBare;
	char denumire[50];
	float pret;
	int stoc;
	Data dataExpirare;
} Produs;

//obtinere total elemente de tip Produs
int getNrProduse(FILE* f) {
	int pozCurenta = ftell(f);//pastrarea pozitiei curente

	fseek(f, 0, SEEK_END);
	int totalOcteti = ftell(f);

	fseek(f, pozCurenta, SEEK_SET);

	return totalOcteti / sizeof(Produs);
}

void sortareFisierDupaDenumireSiPret(char numeFisier[20]) {
	FILE* f = fopen(numeFisier, "rb+");

	//fct strcmp(s1,s2) - compara 2 siruri de caractere si returneaza un nr ...face parte din libraria string.h
	//0 -> daca sirurile sunt identice
	//<0 -> daca s1 < s2 --> adica daca sunt in ordine alfabetica s1 si s2
	//>0 -> daca s1 > s2 --> adica daca NU sunt in ordine alfabetica s1 si s2 ==> asta o sa folosim NOI mai jos.

	Produs a, b;
	int nrProduse = getNrProduse(f);

	if (f != NULL) {
		for (int i = 0; i < nrProduse - 1; i++) {
			fseek(f, i * sizeof(Produs), SEEK_SET);
			fread(&a, sizeof(Produs), 1, f);

			for (int j = i + 1; j < nrProduse; j++) {
				fseek(f, j * sizeof(Produs), SEEK_SET);
				fread(&b, sizeof(Produs), 1, f);

				if (strcmp(a.denumire, b.denumire) > 0 || (strcmp(a.denumire, b.denumire) == 0 && a.pret > b.pret)) {
					fseek(f, i * sizeof(Produs), SEEK_SET);
					fwrite(&b, sizeof(Produs), 1, f);

					fseek(f, j * sizeof(Produs), SEEK_SET);
					fwrite(&a, sizeof(Produs), 1, f);
				}
			}
		}
		fclose(f);
	}
	else {
		printf("\nFisierul %s nu exista!", numeFisier);
	}
}

void sortareFisierInMemorieInternaDupaDenumireSiPret(char numeFisier[20]) {
	FILE* f = fopen(numeFisier, "rb+");
	if (f != NULL) {
		int nr = getNrProduse(f);

		Produs aux;
		Produs* pointerProduse = (Produs*)malloc(sizeof(Produs) * nr);
		fread(pointerProduse, sizeof(Produs), nr, f);//scriem in vectorul dinamic de produse toate elementele din fisier.

		for (int i = 0; i < nr - 1; i++) {
			for (int j = i + 1; j < nr; j++)
			{
				if (strcmp(pointerProduse[i].denumire, pointerProduse[j].denumire) > 0 ||
					(strcmp(pointerProduse[i].denumire, pointerProduse[j].denumire) == 0 && pointerProduse[i].pret > pointerProduse[j].pret)) {
					aux = pointerProduse[i];
					pointerProduse[i] = pointerProduse[j];
					pointerProduse[j] = aux;
				}
			}
		}

		rewind(f);
		fwrite(pointerProduse, sizeof(Produs), nr, f);//suprascrii tot fisierul cu vectorul tau


		fclose(f);
		free(pointerProduse);
	}
	else {
		printf("\nFisierul %s nu exista!", numeFisier);
	}
}

void citireProdus(Produs* p) {
	printf("\nCod produs: ");
	scanf("%d", &p->codDeBare);

	printf("Denumire produs: ");

	getchar();
	fgets(p->denumire, 50, stdin);
	//scanf_s("%s", p.denumire, 50);

	printf("Pret produs: ");
	scanf("%f", &p->pret);
	printf("Stoc produs: ");
	scanf("%d", &p->stoc);

	printf("Data expirare produse -> zi: ");
	scanf("%d", &p->dataExpirare.zi);
	printf("Data expirare produse ->  luna: ");
	scanf("%d", &p->dataExpirare.luna);
	printf("Data expirare produse -> an: ");
	scanf("%d", &p->dataExpirare.an);
}

void scriereInFisierBinarInModSecvential(char numeFisier[20]) {
	FILE* f = fopen(numeFisier, "ab");//deschide fisierul in mod de adaugare binara

	if (f != NULL) {
		int continuare = 1;
		while (continuare == 1)
		{
			Produs p;

			citireProdus(&p);

			fwrite(&p, sizeof(Produs), 1, f);

			printf("\nDoriti sa continuati? (1/0) ");
			scanf("%d", &continuare);
		}

		fclose(f);
	}
}

void modificareProdus(char numeFisier[20], int codBare) {
	FILE* f = fopen(numeFisier, "rb+");
	int nrProduse = getNrProduse(f);

	if (f != NULL)
	{
		Produs p;
		int ok = 0;
		for (int i = 0; i < nrProduse; i++) {
			fread(&p, sizeof(p), 1, f);
			if (codBare == p.codDeBare) {
				p.stoc += 20;
				p.pret -= 2;

				//suprasrie zona de memorie din fisier
				fseek(f, i * sizeof(p), SEEK_SET);
				fwrite(&p, sizeof(p), 1, f);

				ok = 1;
				break;
			}
		}

		if (ok == 1) {
			printf("Produsul cu codul de bare %d a fost modificat.\n", codBare);
		}
		else {
			printf("Produsul cu codul de bare %d nu exista.\n", codBare);
		}
		fclose(f);
	}
	else {
		printf("\nFisierul %s nu exista!", numeFisier);
	}
}

//afisarea unui element de tip produs
void afisareProdus(Produs p) {
	printf("\nProdusul %s cu codul %d are pretul %.2f si expira la data de %d.%d.%d. Avem un stoc de %d produse de acest tip.\n",
		p.denumire, p.codDeBare, p.pret, p.dataExpirare.zi, p.dataExpirare.luna, p.dataExpirare.an, p.stoc);
}

void afisareProduseDinFisierBinar(char numeFisier[20]) {
	FILE* f = fopen(numeFisier, "rb");

	Produs p;
	if (f != NULL) {
		//sau puteati sa faceti ca in ex 1 cu !feof(f)
		while (fread(&p, sizeof(Produs), 1, f) == 1) {
			afisareProdus(p);
		}

		fclose(f);
	}
	else {
		printf("\nFisierul %s nu exista!", numeFisier);
	}
}

void generareRaport(char numeFisier[20]) {
	FILE* f = fopen(numeFisier, "rb");

	FILE* g = fopen("RaportProduse.txt", "w");

	Produs p;
	if (f != NULL) {
		while (fread(&p, sizeof(Produs), 1, f) == 1) {
			fprintf(g, "\nProdusul %s cu codul %d are pretul %.2f si expira la data de %d.%d.%d. Avem un stoc de %d produse de acest tip.\n",
				p.denumire, p.codDeBare, p.pret, p.dataExpirare.zi, p.dataExpirare.luna, p.dataExpirare.an, p.stoc);
		}
		fclose(f);
	}
	else {
		printf("\nFisierul %s nu exista!", numeFisier);
	}
}

int main() {
	char numeFisier[20];
	printf("Numele fisierului binar secvential: ");
	scanf("%s", numeFisier);

	printf("Vreti sa scrieti ceva in fisierul binar?(0/1)\n");
	int ok = 0;
	scanf("%d", &ok);
	if (ok == 1) {
		scriereInFisierBinarInModSecvential(numeFisier);
	}

	afisareProduseDinFisierBinar(numeFisier);

	printf("\n\nAfisare fisier dupa sortare.\n\n");

	//sortareFisierDupaDenumireSiPret(numeFisier);
	sortareFisierInMemorieInternaDupaDenumireSiPret(numeFisier);

	afisareProduseDinFisierBinar(numeFisier);

	printf("\n=== modificare produs ===\n");
	modificareProdus(numeFisier, 199);

	printf("\n=== generare raport ===\n");

	generareRaport(numeFisier);
	return 0;
}
