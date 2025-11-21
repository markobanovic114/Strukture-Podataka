#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BROJ_BODOVA 75
#define FILEPATH "Vjezba1/studenti.txt"


typedef struct {
	char name[15];
	char surname[15];
	int aps_points;
	float rel_points;
}_Student;				//deklaracija i definicija strukture

int calculate_rows(char*);
_Student* student_input(char*, int);
void student_output(_Student*, int);			//prototipi funkcija

int main() {
	int num_rows;
	char* filename = FILEPATH;
	_Student* students;

	num_rows = calculate_rows(filename);
	if (num_rows == -1) {
		printf("Datoteka nema redova!\n");
		return 1;
	}

	students = student_input(filename, num_rows);
	if (students == NULL) {
		printf("Neuspjelo ucitavanje studenata.\n");
		return 1;
	}
	student_output(students, num_rows);
	printf("Broj redaka je: %d", num_rows);

	free(students);				//oslobadanje memorije
	return 0;
}

int calculate_rows(char* file) {
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {										//provjera validnosti pointera
		printf("Ne mogu otvoriti datoteku!\n");
		return -1;
	}

	int counter = 0;
	char buffer[1020];
	while (fgets(buffer, 1020, fp)) {				//citanje redova
		counter++;
	}

	if (counter == 0) {	
		fclose(fp);
		return -1;
	}

	return counter;
}

_Student* student_input(char* file, int num_rows) {
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {
		printf("Ne mogu otvoriti datoteku!\n");
		return NULL;
	}

	_Student* s = malloc(num_rows * sizeof(_Student));				//dinamicka alokacija memorije za studente
	if (s == NULL) {
		printf("Nema dovoljno memorije!\n");
		fclose(fp);
		return NULL;
	}

	for (int i = 0; i < num_rows; i++) {										//upisivanje podataka u strukture odgovarajucih studenata
		fscanf(fp, "%14s %14s %d", s[i].name, s[i].surname, &s[i].aps_points);
		s[i].rel_points = ((float)s[i].aps_points / BROJ_BODOVA) * 100;			//racunanje relativnih bodova i upis
	}

	fclose(fp);
	return s;
}

void student_output(_Student* s, int num_rows) {
	printf("%-20s %-20s %-20s %-10s\n", "Ime", "Prezime", "Bodovi", "Relativni");
	for (int i = 0; i < num_rows; i++) {
		printf("%-20s %-20s %-20d %.2f%%\n", s[i].name, s[i].surname, s[i].aps_points, s[i].rel_points);		//ispis podataka
	}
}


