#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILEPATH "Vjezba5/izraz.txt"
#define MAX 100

typedef struct Poly {
	int coeff, pow;
	struct Poly* next;
}Poly;

void printpoly(Poly*);
Poly* create(int, int);
Poly* add(Poly*, int, int);
char* read_file();

int main() {
	Poly* head = NULL;
	Poly* second = NULL;
	Poly* first = NULL;
	char* buffer = read_file();
	int coeff, pow;
	int i = 0;
	printf("%s\n", buffer);

	while (buffer[i] != ' ' && buffer[i] != '\0') {
		coeff = buffer[i++] - '0';
		if (buffer[i] == ' ' || buffer[i] == '\0') break;
		pow = buffer[i++] - '0';
		first = add(first, coeff, pow);
	}

	if (buffer[i] == ' ') i++;

	while (buffer[i] != '\0') {
		coeff = buffer[i++] - '0';
		if (buffer[i] == '\0') break;
		pow = buffer[i++] - '0';
		second = add(second, coeff, pow);
	}

	printpoly(first);
	printf("\n");
	printpoly(second);
}

char* read_file() {
	int j = 0;
	FILE* f1 = fopen(FILEPATH, "r");
	if (!f1) {
		printf("Greska u otvaranju filea!\n");
		return NULL;
	}
	char* buffer = (char*)malloc(MAX * sizeof(char));
	char* real_buffer = (char*)malloc(MAX * sizeof(char));
	if (!buffer || !real_buffer) {
		printf("Greska u alokaciji!\n");
		return NULL;
	}

	while (fgets(buffer, MAX, f1)) {
		for (int i = 0; buffer[i] != '\0'; i++) {
			if ((buffer[i] == ' ') || (buffer[i] == '\n')) {
				continue;
			}
			real_buffer[j++] = buffer[i];
		}
		real_buffer[j++] = ' ';
	}
	real_buffer[j] = '\0';
	free(buffer);
	fclose(f1);
	return real_buffer;
}

Poly* create(int coeff, int pow) {
	Poly* new_poly = (Poly*)malloc(sizeof(Poly));
	if (!new_poly) {
		printf("Greska u alokaciji!\n");
		return NULL;
	}
	new_poly->next = NULL;
	new_poly->coeff = coeff;
	new_poly->pow = pow;

	return new_poly;
}
Poly* add(Poly* head, int coeff, int pow) {
	if (head == NULL) {
		Poly* new_poly = create(coeff, pow);
		if (!new_poly) {
			printf("Greska u alokaciji!\n");
			return NULL;
		}
		return new_poly;
	}

	Poly* temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	Poly* new_poly = create(coeff, pow);
	if (!new_poly) {
		printf("Greska u alokaciji!\n");
		return NULL;
	}
	new_poly->next = temp->next;
	temp->next = new_poly;

	return head;
}
void printpoly(Poly* head) {
	Poly* temp = head;

	while (temp != NULL) {
		printf("%d%d ", temp->coeff, temp->pow);
		temp = temp->next;
	}
} //testna funk