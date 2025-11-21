#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILEPATH_OUT "Vjezba3/upis.txt"
#define FILEPATH_IN "Vjezba3/ispis.txt"

typedef struct Person* P;
typedef struct Person {
	char name[20];
	char surname[20];
	int day, month, year;
	struct Person* next;
}Person;

P add_in_front(P);
P add_to_end(P);
P add_in_front_of(P, char*);
P add_behind_of(P, char*);
P delete_from_list(P, char* surname);
P read_from_file(P);
P create();
int find_in_list(P, char*);
int print_list(P);
int print_in_file(P);
int sort_list(P);

int main() {
	int choice;
	int status_print = 0;
	int status_sort = 0;
	int status_file = 0;
	char sur[20];
	P head = NULL;
	do {
		printf("Izaberite funkciju (0 za izlaz):\n");
		printf("1 - Dodaj na pocetak\n"); 
		printf("2 - Dodaj na kraj\n"); 
		printf("3 - Izbrisi iz liste\n"); 
		printf("4 - Pronadi\n"); 
		printf("5 - Ispisi listu\n"); 
		printf("6 - Dodaj iza clana\n"); 
		printf("7 - Dodaj ispred clana\n"); 
		printf("8 - Ispis u datoteku\n"); 
		printf("9 - Upis iz datoteke\n"); 
		printf("10 - Sortiranje liste\n"); scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			head = add_in_front(head);
			break;
		case 2:
			head = add_to_end(head);
			break;
		case 3:
			printf("Unesite prezime za izbrisati: "); scanf("%19s", sur);
			if (delete_from_list(head, sur) == NULL) {
				return EXIT_FAILURE;
			}
			break;
		case 4:
			printf("Unesite prezime za pronaci: "); scanf("%19s", sur);
			if (find_in_list(head, sur) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
			break;
		case 5:
			status_print = print_list(head);
			if (status_print != 0)
				printf("Doslo je do greske!\n");
			break;
		case 6:
			printf("Unesite prezime: "); scanf("%19s", sur);
			head = add_behind_of(head, sur);
			break;
		case 7:
			printf("Unesite prezime: "); scanf("%19s", sur);
			head = add_in_front_of(head, sur);
			break;
		case 8:
			status_file = print_in_file(head);
			if (status_file != 0)
				printf("Doslo je do greske!\n");
			break;
		case 9:
		{
			head = read_from_file(head);
			break;
		}
		case 10:
			status_sort = sort_list(head);
			if (!status_sort) {
				printf("Lista je sortirana.\n");
			}
			break;
		case 0:
			break;
		default:
			printf("Niste unijeli pravilan broj!\n");
			return 1;
			break;
		}
	} while (choice != 0);

	P temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
	return 0;
}

P create() {
	P new_person = (P)malloc(sizeof(Person));
	if (new_person == NULL) {
		printf("Greska u alociranju memorije!\n");
		return EXIT_FAILURE;
	}

	printf("Unesite ime: ");
	scanf("%19s", new_person->name);
	printf("Unesite prezime: ");
	scanf("%19s", new_person->surname);
	printf("Unesite datum rodenja (dd mm yyyy): ");
	scanf("%d %d %d", &new_person->day, &new_person->month, &new_person->year);
	new_person->next = NULL;

	return new_person;
}

P add_in_front(P head) {
	P person_to_add = create();
	if (person_to_add == NULL) {
		printf("Greska u stvaranju osobe!\n");
		return head;
	}

	person_to_add->next = head;
	return person_to_add;
}

P add_to_end(P head) {
	P person_to_add = create();
	if (person_to_add == NULL) {
		printf("Greska u stvaranju osobe!\n");
		return head;
	}
	if (head == NULL) {
		return person_to_add;
	}

	P temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	person_to_add->next = temp->next;
	temp->next = person_to_add;
	return head;
}

int print_list(P head) {
	if (head == NULL) {
		printf("Greska! Nema ljudi za ispisati!\n");
		return EXIT_FAILURE;
	}
	P temp = head;
	printf("%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	printf("-----------------------------------------------------------------------\n");

	do {
		printf("%-20s %-20s %02d.%02d.%04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
		temp = temp->next;
	} while (temp != NULL);

	return EXIT_SUCCESS;
}

P delete_from_list(P head, char* surname) {
	P temp = head;
	P previous = NULL;
	if (head == NULL) {
		printf("Lista je prazna!\n");
		return EXIT_FAILURE;
	}

	while (temp != NULL && strcmp(temp->surname, surname) != 0) {
		previous = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		printf("Greska! Ne postoji osoba s tim prezimenom!\n");
		return head;
	}

	if (previous == NULL) {
		head = temp->next;
	}
	else {
		previous->next = temp->next;
	}

	free(temp);
	return head;
}

int find_in_list(P head, char* surname) {
	if (head == NULL) {
		printf("Greska! Nema takve osobe!\n");
		return EXIT_FAILURE;
	}
	P temp = head;

	while (temp != NULL) {
		if (strcmp(temp->surname, surname) == 0) {
			printf("Pronadena je osoba: %s %s\trodena: %02d %02d %04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
			return 0;
		}
		temp = temp->next;
	}

	printf("Greska! Nema takve osobe!\n");
	return EXIT_FAILURE;
}

P add_in_front_of(P head, char* surname) {
	P curr = head;
	P prev = NULL;

	while (curr != NULL) {
		if (strcmp(curr->surname, surname) == 0) {
			P new_person = create();
			if (new_person == NULL) {
				return head;
			}
			if (prev == NULL) {
				new_person->next = head;
				return new_person;
			}
			prev->next = new_person;
			new_person->next = curr;
			return head;
		}
		prev = curr;
		curr = curr->next;
	}

	printf("Osoba s prezimenom %s nije pronadena!\n", surname);
	return head;
}

P add_behind_of(P head, char* surname) {
	P curr = head;

	while (curr != NULL) {
		if (strcmp(curr->surname, surname) == 0) {
			P new_person = create();
			new_person->next = curr->next;
			curr->next = new_person;
		}
		curr = curr->next;
	}

	return head;
}

int print_in_file(P head) {
	FILE* f1 = fopen(FILEPATH_IN, "w");
	if (f1 == NULL) {
		printf("Greska u otvaranju filea!");
		return EXIT_FAILURE;
	}

	P temp = head;
	fprintf(f1, "%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	fprintf(f1, "-----------------------------------------------------------------------\n");
	while (temp != NULL) {
		fprintf(f1, "%-20s %-20s %02d.%02d.%04d\n", temp->name, temp->surname, temp->day, temp->month, temp->year);
		temp = temp->next;
	}

	fclose(f1);
	return EXIT_SUCCESS;
}

P read_from_file(P head) {
	FILE* f1 = fopen(FILEPATH_OUT, "r");
	if (f1 == NULL) {
		printf("Greska u otvaranju filea!\n");
		return EXIT_FAILURE;
	}
	int read;
	P temp = head;
	char buffer[1020];
	fgets(buffer, 1020, f1);
	fgets(buffer, 1020, f1);

	while (1) {
		P new_person = (P)malloc(sizeof(Person));
		if (new_person == NULL) {
			printf("Greska u alociranju!\n");
			return EXIT_FAILURE;
		}
		if ((read = fscanf(f1, "%19s %19s %d.%d.%d", new_person->name, new_person->surname, &new_person->day, &new_person->month, &new_person->year)) != 5) {
			free(new_person);
			break;
		}
		new_person->next = NULL;

		if (temp == NULL) {
			head = new_person;
			temp = new_person;
			continue;
		}

		new_person->next = temp->next;
		temp->next = new_person;
		temp = temp->next;
	}

	fclose(f1);
	return head;
}

int sort_list(P head) {
	if (head == NULL || head->next == NULL) {
		printf("Nema dovoljno elemenata za sortiranje!\n");
		return EXIT_FAILURE;
	}

	int swapped = 1;
	P prvi;
	P zadnji = NULL;

	while (swapped) {
		swapped = 0;
		prvi = head;

		while (prvi->next != zadnji) {
			if (strcmp(prvi->surname, prvi->next->surname) > 0) {
				char temp_name[20], temp_surname[20];
				int temp_day, temp_month, temp_year;

				strcpy(temp_name, prvi->name);
				strcpy(temp_surname, prvi->surname);
				temp_day = prvi->day;
				temp_month = prvi->month;
				temp_year = prvi->year;

				strcpy(prvi->name, prvi->next->name);
				strcpy(prvi->surname, prvi->next->surname);
				prvi->day = prvi->next->day;
				prvi->month = prvi->next->month;
				prvi->year = prvi->next->year;

				strcpy(prvi->next->name, temp_name);
				strcpy(prvi->next->surname, temp_surname);
				prvi->next->day = temp_day;
				prvi->next->month = temp_month;
				prvi->next->year = temp_year;

				swapped = 1;
			}

			prvi = prvi->next;
		}
	}
	return EXIT_SUCCESS;
}