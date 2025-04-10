#include<stdio.h>
#include<string.h>


#pragma warning (disable:4996)

struct absente
{
	int absCurs;
	char numeCurs[50];

	int absSeminar;
	char numeSeminar[50];
};

struct student {
	char nume[100];
	absente absente[14];
	int varsta;
	char telefon[30];
	float medieAnPrecedent;
};

void afisareStudent(student s) {
	printf("nume = %s\n", s.nume);
	printf("varsta = %d\n", s.varsta);
	printf("telefon = %s\n", s.telefon);
	printf("medieAnPrecedent = %f\n", s.medieAnPrecedent);
	printf("Absente: \n");

	for (int i = 0; i < 14; i++) {
		printf("numeCurs = %s\n", s.absente[i].numeCurs);
		printf("curs = %d\n", s.absente[i].absCurs);

		printf("numeSeminar = %s\n", s.absente[i].numeSeminar);
		printf("seminar = %d\n", s.absente[i].absSeminar);
	}
}

void afisareStudentInFisTxt(student s, FILE* f) {
	fprintf(f, "%s\n", s.nume);
	fprintf(f, "%d\n", s.varsta);
	fprintf(f, "%s\n", s.telefon);
	fprintf(f, "%f\n", s.medieAnPrecedent);

	for (int i = 0; i < 14; i++) {
		fprintf(f, "%s\n", s.absente[i].numeCurs);
		fprintf(f, "%d\n", s.absente[i].absCurs);

		fprintf(f, "%s\n", s.absente[i].numeSeminar);
		fprintf(f, "%d\n", s.absente[i].absSeminar);
	}
}

void citireStudent(student& s) {
	printf("nume = "); gets_s(s.nume, 100);

	printf("varsta = ");  scanf("%d", &s.varsta);

	getchar();
	printf("telefon =");  gets_s(s.telefon, 30);

	printf("medieAnPrecedent = ");  scanf("%f", &s.medieAnPrecedent);
	getchar();

	char aux[50];
	for (int i = 0; i < 14; i++) {
		sprintf(aux, "Curs %d", i);
		strcpy(s.absente[i].numeCurs, aux);
		s.absente[i].absCurs = 0;

		sprintf(aux, "Seminar %d", i);
		strcpy(s.absente[i].numeSeminar, aux);
		s.absente[i].absSeminar = 0;
	}
}

void main() {
	student s1, s2;

	citireStudent(s1);

	printf("\n\n---afisare stud 1---\n\n");
	afisareStudent(s1);

	citireStudent(s2);

	printf("\n\n---afisare stud 2---\n\n");
	afisareStudent(s2);


	FILE* f, * g;
	f = fopen("student.bin", "wb");
	if (f == NULL) {
		printf("fisierul nu poate fi deschis\n");
		return;
	}

	fwrite(&s1, sizeof(student), 1, f);
	fwrite(&s2, sizeof(student), 1, f);

	fclose(f);


	g = fopen("studenti.txt", "w");
	f = fopen("student.bin", "rb");

	student temp;

	while (fread(&temp, sizeof(student), 1, f) == 1)
	{
		afisareStudentInFisTxt(temp, g);
	}

	fclose(f); fclose(g);
}