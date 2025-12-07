#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LONG 50
#define MAX_SHORT 20

typedef struct Receipt {
	char* name;
	int amount;
	float price;
	struct Receipt* next;
}Receipt;
typedef struct ReceiptMain {
	char* date;
	struct ReceiptMain* next;
	struct Receipt* nextReceipt;
}ReceiptMain;

Receipt* create_dummy_head();
ReceiptMain* create_main_head();
ReceiptMain* createMain(char*);
Receipt* createReceipt(char*, int, float);
int read_main_file(ReceiptMain*);
int read_lines(char*, ReceiptMain*);
int add_to_main(ReceiptMain*, ReceiptMain*);
int add_receipt(Receipt*, Receipt*);
char* create_string(char*);
void print(ReceiptMain*);
float find(ReceiptMain*, char*, char*, char*);
void free_memory(ReceiptMain*);


int main() {
	ReceiptMain* headMain = create_main_head();
	if (!headMain) {
		return EXIT_FAILURE;
	}
	int check = 0; float money = 0;

	check = read_main_file(headMain);
	if (check == 1) {
		return EXIT_FAILURE;
	}
	puts("Procitani su sljedeci datumi:");
	print(headMain);

	char* date1 = (char*)malloc(MAX_SHORT);
	char* date2 = (char*)malloc(MAX_SHORT);
	char* item = (char*)malloc(MAX_SHORT);
	if (!date1 || !date2 || !item) {
		printf("Greska u alokaciji stringa!\n");
		return EXIT_FAILURE;
	}
	puts("\nUnesite pocetno razdoblje datuma (yyyy-mm-dd): "); fgets(date1, MAX_SHORT, stdin); date1[strcspn(date1, "\n")] = '\0';
	puts("Unesite krajnje razdoblje datuma (yyyy-mm-dd): "); fgets(date2, MAX_SHORT, stdin); date2[strcspn(date2, "\n")] = '\0';
	puts("Unesite ime proizvoda: "); fgets(item, MAX_SHORT, stdin); item[strcspn(item, "\n")] = '\0';
	
	money = find(headMain, date1, date2, item);
	printf("Potroseno je %.2f eura.\n", money);

	free(date1); free(date2); free(item);
	free_memory(headMain);
}

int read_main_file(ReceiptMain* headM) {
	FILE* f1 = fopen("Vjezba6/racuni.txt", "r");
	char* buffer = (char*)malloc(MAX_LONG * sizeof(char));
	int check = 0;

	if (!f1) {
		printf("Greska u otvaranju filea f1!\n");
		return EXIT_FAILURE;
	}
	if (!buffer) {
		printf("Greska u alokaciji buffera!\n");
		return EXIT_FAILURE;
	}

	while (fscanf(f1, "%s", buffer) == 1) {
		check = read_lines(buffer, headM);
		if (check == 1) {
			return EXIT_FAILURE;
		}
		memset(buffer, 0, MAX_LONG);
	}

	fclose(f1);
	free(buffer);

	return EXIT_SUCCESS;
}

int read_lines(char* buffer, ReceiptMain* headM) {
	int amount; float price;
	char* date_buffer = (char*)malloc(MAX_SHORT); 
	char* name = (char*)malloc(MAX_SHORT);
	if (!name || !date_buffer) {
		printf("Greska u alokaciji buffera!\n");
		return EXIT_FAILURE;
	}
	char* string = create_string(buffer);
	FILE* f2 = fopen(string, "r");
	if (!f2) {
		printf("Greska u otvaranju filea f2!\n");
		return EXIT_FAILURE;
	}

	fgets(date_buffer, MAX_SHORT, f2);
	date_buffer[strcspn(date_buffer, "\n")] = '\0';

	ReceiptMain* node_to_add = createMain(date_buffer);
	if (!node_to_add) {
		return EXIT_FAILURE;
	}
	node_to_add->nextReceipt = create_dummy_head();
	if (!node_to_add->nextReceipt) {
		return EXIT_FAILURE;
	}

	while (fscanf(f2, "%14s %d %f", name, &amount, &price) == 3) {
		Receipt* receipt_node = createReceipt(name, amount, price);
		add_receipt(node_to_add->nextReceipt, receipt_node);
	}
	add_to_main(headM, node_to_add);

	free(string);
	fclose(f2);
	free(name);
	free(date_buffer);

	return EXIT_SUCCESS;
}

int add_to_main(ReceiptMain* headM, ReceiptMain* node) {
	ReceiptMain* temp = headM->next;
	ReceiptMain* prev = headM;

	while (temp != NULL && strcmp(temp->date, node->date) < 0) {
		prev = temp;
		temp = temp->next;
	}

	node->next = temp;
	prev->next = node;

	return EXIT_SUCCESS;
}

int add_receipt(Receipt* MainNode, Receipt* ReceiptNode) {
	Receipt* temp = MainNode->next;
	Receipt* prev = MainNode;

	while (temp != NULL && strcmp(temp->name, ReceiptNode->name) < 0) {
		prev = temp;
		temp = temp->next;
	}

	ReceiptNode->next = temp;
	prev->next = ReceiptNode;

	return EXIT_SUCCESS;
}

ReceiptMain* createMain(char* date_buffer) {
	ReceiptMain* newnode = (ReceiptMain*)malloc(sizeof(ReceiptMain));
	if (!newnode) {
		printf("Greska u stvaranju glavnog node-a!\n");
		return NULL;
	}
	newnode->date = _strdup(date_buffer);
	newnode->next = NULL;
	newnode->nextReceipt = NULL;

	return newnode;
}

Receipt* createReceipt(char* name, int amount, float price) {
	Receipt* new_receipt = (Receipt*)malloc(sizeof(Receipt));
	if (!new_receipt) {
		printf("Greska u alokaciji!2\n");
		return NULL;
	}

	new_receipt->name = _strdup(name);
	new_receipt->amount = amount;
	new_receipt->price = price;
	new_receipt->next = NULL;

	return new_receipt;
}

void print(ReceiptMain* headM) {
	ReceiptMain* temp = headM->next;

	while (temp != NULL) {
		printf("%s\n", temp->date);
		temp = temp->next;
	}
}

void print_receipt(char* trazeni, ReceiptMain* headM) {
	ReceiptMain* temp = headM->next;

	while (temp != NULL) {
		if (strcmp(temp->date, trazeni) == 0) {
			Receipt* tempR = temp->nextReceipt->next;
			while (tempR != NULL) {
				printf("%s %d %.2f\n", tempR->name, tempR->amount, tempR->price);
				tempR = tempR->next;
			}
		}
		temp = temp->next;
	}
}

Receipt* create_dummy_head() {
	Receipt* dummy_head = (Receipt*)malloc(sizeof(Receipt));
	if (!dummy_head) {
		printf("Greska u alokaciji receipt head-a!\n");
		return NULL;
	}
	dummy_head->amount = 0;
	dummy_head->name = NULL;
	dummy_head->price = 0;
	dummy_head->next = NULL;
	
	return dummy_head;
}

ReceiptMain* create_main_head() {
	ReceiptMain* MainHead = (ReceiptMain*)malloc(sizeof(ReceiptMain));
	if (!MainHead) {
		printf("Greska u alokaciji glavnog head-a!\n");
		return NULL;
	}
	MainHead->date = NULL;
	MainHead->next = NULL;
	MainHead->nextReceipt = NULL;

	return MainHead;
}

char* create_string(char* text) {
	char* prefix = "Vjezba6/";
	text[strcspn(text, "\n")] = '\0';
	int str_len = (int)strlen(text) + (int)strlen(prefix) + 1;

	char* string = (char*)malloc(str_len);
	if (!string) {
		printf("Greska u alokaciji stringa!\n");
		return NULL;
	}
	strcpy(string, prefix);
	strcat(string, text);

	return string;
}

float find(ReceiptMain* headM, char* date1, char* date2, char* item) {
	float money_spent = 0;
	ReceiptMain* temp = headM->next;

	while (temp != NULL) {
		if (strcmp(temp->date, date1) >= 0 && strcmp(temp->date, date2) <= 0) {
			Receipt* tempR = temp->nextReceipt->next;
			while (tempR != NULL) {
				if (strcmp(tempR->name, item) == 0) {
					money_spent += tempR->price * tempR->amount;
				}
				tempR = tempR->next;
			}
		}
		temp = temp->next;
	}

	return money_spent;
}

void free_memory(ReceiptMain* headM) {
	ReceiptMain* tempMain = headM;
	while (tempMain != NULL) {
		ReceiptMain* nextMain = tempMain->next;

		Receipt* tempR = tempMain->nextReceipt;
		while (tempR != NULL) {
			Receipt* nextR = tempR->next;
			if (tempR->name) free(tempR->name);
			free(tempR);
			tempR = nextR;
		}

		if (tempMain->date) free(tempMain->date);

		free(tempMain);
		tempMain = nextMain;
	}
}