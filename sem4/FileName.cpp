//rewind(f) -> te repozitionezi la inceputul fisierului. 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma warning(disable:4996)

struct Animal {
	int key;
	char denumire[100];
	int varsta;
	double greutate;
	char culoare[20];
	int esteSetat;//val 0/1 - pentru a indica daca o locatie din fisier este ocupata de un animal
};

void afisareAnimal(Animal a) {
	printf("\n======\n");
	printf("key = % d\n", a.key);
	printf("denumire = %s\n", a.denumire);
	printf("varsta = %d\n", a.varsta);
	printf("greutate = %.2lf\n", a.greutate);
	printf("culoare = %s\n", a.culoare);
	printf("esteSetat = %d\n", a.esteSetat);
	printf("======\n");
}

void citireAnimal(Animal& a) {
	printf("denumire = ");  getchar();  gets_s(a.denumire);
	printf("varsta = "); scanf("%d", &a.varsta);
	printf("greutate = "); scanf("%lf", &a.greutate);
	printf("culoare = "); scanf("%s", a.culoare);
	a.esteSetat = 1;
}

int aflaCateAnimaleSuntInFiser(FILE* f) {
	//ftell -> returneaza pozitia curenta a cursorului in fisier
	int pozCurenta = ftell(f);//pastram pozita curenta;

	fseek(f, 0, SEEK_END);//se pozitioneaza la sf fisierului

	int nrAnimale = ftell(f) / sizeof(Animal); //daca tu ai 4 animale in fisier=> ftell va fi 560  => 560/140 = 4 animale

	fseek(f, pozCurenta, SEEK_SET);//se pozitioneaza la pozitia initiala

	return nrAnimale;
}

Animal createEmptyAnimal() {
	Animal a;
	strcpy(a.culoare, "culoareEmpty");
	strcpy(a.denumire, "denumireEmpty");
	a.varsta = 0;
	a.esteSetat = 0;
	a.greutate = 0.0;
	a.key = 0;

	return a;
}
void main() {
	//Afisarea informatiilor in format tabel

	//FILE *f = fopen("ceva.txt", "w");
	//fprintf(f, "%4s %-16s %3s %30s", "Crt.", "BBB", "AAAA", "Suma");
	//printf("%4s %-16s %3s %30s", "Crt.", "BBB", "AAAA", "Suma");

	//fclose(f);


	//long int ftell(FILE *fp); - returneaza pozitia curenta;

	//int fseek(FILE *fp, long int offset, int origin);

	//fp - fisierul peste care se aplica operatia fseek;

	//offset - numarul de bytes fata de origin(al 3 lea parametru);
	//poate fi nr. pozitiv(pt mersul inainte prin fisier) sau negativ(pt mersul inapoi prin fisier);

	//origin - originea punctului fata de care se aplica offsetul; 
	//Ex: SEEK_SET - inceputul fisierului
	//Ex: SEEK_CUR - pozitia curenta a fisierului
	//Ex: SEEK_END - sfarsitul fisierului

	//fseek - returneaza 0 daca mutarea a fost efectuata cu succes, altfel o alta val !=0

	FILE* fisBinar = fopen("fisBinarAnimale.bin", "wb+");
	if (fisBinar == NULL) {
		printf("ERROR: fisBinarAnimale.bin nu s-a putut deschide.");
		exit(1);
	}


	while (!feof(stdin)) {//atunci cand se cere ca sfarsitul introducerii sa fie marcat standard
		printf("\n\nAlegeti o optiune din meniu:\n");
		printf("1) Salvati un animal in fisier la o anumita pozitie.\n");
		printf("2) Modificati un animal din fisier de la o anumita pozitie.\n");
		printf("3) Stergeti un animal din fisier de la o anumita pozitie.\n");
		printf("4) Afisati un animal din fisier de la o anumita pozitie.\n");
		printf("5) Exit.\n");

		int optiune;
		printf("Introduceti optiunea dorita: "); scanf("%d", &optiune);

		switch (optiune)
		{
		case 1:
		{
			int key;
			printf("Introduceti un animal de la tastatura:\n");
			printf("Key: "); scanf("%d", &key);
			int nrAnimale = aflaCateAnimaleSuntInFiser(fisBinar);

			if (key >= 0) {
				Animal x = createEmptyAnimal();

				if (key >= nrAnimale) {
					//alocam memorie noua daca cheia introdusa > nr de animale din fisier

					x.esteSetat = 0;

					fseek(fisBinar, 0, SEEK_END);//cand fis e gol si te pozitionezi la sf => te pozitionezi la inceputul fisierului.


					for (int i = 0; i < (key + 1) - nrAnimale; i++) {
						fwrite(&x, sizeof(Animal), 1, fisBinar);
					}

					//sau in loc de for(le scrii pe toate odata): fwrite(&x, sizeof(Animal), (key + 1) - nrAnimale, fisBinar);
				}

				//se pozitioneaza cursorul la adresa key * sizeof(Animal)
				fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

				//se citeste animalul de la adreasa: key * sizeof(Animal)
				fread(&x, sizeof(Animal), 1, fisBinar);

				if (x.esteSetat == 1) {
					printf("La cheia introdusa exista deja un animal salvat. Nu se mai poate adauga unul.\n");
				}
				else {
					x.key = key;
					citireAnimal(x);

					//se pozitioneaza cursorul la adresa key * sizeof(Animal)
					fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

					//se scrie Animalul in fisier la adresa key * sizeof(Animal)
					fwrite(&x, sizeof(Animal), 1, fisBinar);

				}
			}

			break;
		}
		case 2: {
			Animal x = createEmptyAnimal();

			int key;
			printf("Key: "); scanf("%d", &key);

			//se pozitioneaza cursorul la adresa key * sizeof(Animal)
			fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

			//se citeste animalul de la adreasa: key * sizeof(Animal)
			fread(&x, sizeof(Animal), 1, fisBinar);

			if (x.esteSetat == 0) {
				printf("La cheia introdusa nu exista niciun animal. NU se poate modifica nimic.\n");
			}
			else {
				citireAnimal(x);

				//se pozitioneaza cursorul la adresa key * sizeof(Animal)
				fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

				//se scrie Animalul in fisier la adresa key * sizeof(Animal)
				fwrite(&x, sizeof(Animal), 1, fisBinar);

			}

			break;
		}
		case 3: {
			Animal x = createEmptyAnimal();

			int key;
			printf("Key: "); scanf("%d", &key);


			//se pozitioneaza cursorul la adresa key * sizeof(Animal)
			fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

			//se citeste animalul de la adreasa: key * sizeof(Animal)
			fread(&x, sizeof(Animal), 1, fisBinar);

			if (x.esteSetat == 0) {
				printf("La cheia introdusa nu exista niciun animal. NU se poate sterge nimic.\n");
			}
			else {
				x.esteSetat = 0;

				//se pozitioneaza cursorul la adresa key * sizeof(Animal)
				fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

				//se scrie Animalul in fisier la adresa key * sizeof(Animal)
				fwrite(&x, sizeof(Animal), 1, fisBinar);

				printf("S-a sters animalul de la cheia: %d\n", key);
			}

			break;
		}
		case 4: {
			Animal x = createEmptyAnimal();

			int key;
			printf("Key: "); scanf("%d", &key);

			//se pozitioneaza cursorul la adresa key * sizeof(Animal)
			fseek(fisBinar, key * sizeof(Animal), SEEK_SET);

			//se citeste animalul de la adreasa: key * sizeof(Animal)
			fread(&x, sizeof(Animal), 1, fisBinar);

			if (x.esteSetat == 0) {
				printf("La cheia introdusa nu exista niciun animal. NU se poate afisa nimic.\n");
			}
			else {
				afisareAnimal(x);
			}

			break;
		}
		case 5:
		{
			printf("Aplicatia s-a oprit.\n");
			exit(44);
		}
		default:
			printf("Optiunea introdusa nu exista.\n");
		}
	}
}