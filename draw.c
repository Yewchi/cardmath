#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define DECK_SIZE 60

#define CARD_DRAWN 0xAA
#define CARD_IN_DECK 0xBB

char default_deck_list_name[] = {'c', 'a', 'r', 'd', 's', '.', 't', 'x', 't'};

typedef struct CARD card;
struct CARD {
	int num;
	card* next;
};

typedef enum parse_result {
	FULL_PARSE,
	PARTIAL_PARSE,
	FAILED_PARSE
} PARSE_RESULT;

typedef enum bool {
	FALSE,
	TRUE,
} BOOL;

PARSE_RESULT parse_deck(char* filename, char** deck, int deck_size) {
	FILE* fp = fopen(filename, "r");
	int ln_len = 200;
	char ln[ln_len];
	int cards_read = 0;

	int n = -1;
	while(fgets(ln, ln_len, fp) != NULL && n++ < deck_size) {
		int this_card_num;
		int magnitude = 0;

		if(ln[0] == '\n') { 
			continue; 
		} else if(ln[strlen(ln)-1] == '\n') {
			ln[strlen(ln)-1] = '\0';
		}

		sscanf(ln, "%d", &this_card_num);

		int mag_getter = this_card_num;
		while(mag_getter > 0) {
			mag_getter = mag_getter / 10;
			++magnitude;
		}

		for(int j = 0; j < this_card_num; j++) {
			deck[n] = (char*)malloc(sizeof(char)*strlen(ln)+1);
			strcpy(deck[n], (ln+(magnitude+1)));
			if (j < this_card_num-1) { ++n; }
		}
	}
}

void free_deck_list(char** deck_list, int deck_size) {
	for(int n = 0; n < deck_size; n++) {
		free(deck_list[n]);
	}
	free(deck_list);
}


	
int main(int argc, char** argv) {
	char str[5];
	int* cards;
	char* deck_list_fn = default_deck_list_name;
	char** deck_list;
	int cards_picked = 0;
	int deck_size = DECK_SIZE;
	BOOL using_numbers = FALSE;
	time_t t;

	if(argc > 1) {
		if(strcmp("-n", argv[1]) == 0) {
			using_numbers = TRUE;
			if(argc > 2) {
				deck_size = atoi(argv[2]);
			}
		} else {
			deck_size = atoi(argv[1]);
			if (argc > 2) {
				deck_list_fn = argv[2];
			}
		}
	}

	// Check if default avail, ask for use numbers if unavil.
	if(!using_numbers && access(deck_list_fn, R_OK) == -1) {
		printf("Cannot find file \'%s\'..\n", deck_list_fn);
		printf("Continue? (y/n): ");
		fgets(str, sizeof(str), stdin);
		if(str[0] == 'y' || str[0] == 'Y') {
			using_numbers = TRUE;
		} else {
			printf("Exiting...\n");
			return EXIT_SUCCESS;
		}
	}

	// Make/parse deck_list
	deck_list = (char**)malloc(sizeof(char*)*deck_size);
	if(using_numbers) {
		printf("Using card#-wise draws..\n");
		for(int n = 0; n < deck_size; n++) {
			deck_list[n] = (char*)malloc(sizeof(char)*4);
			itoa(n+1, deck_list[n], 10);
		}
	} else {
		printf("Parsing deck list file \'%s\' of size %d.\n", deck_list_fn, deck_size);
		if(parse_deck(deck_list_fn, deck_list, deck_size) == FAILED_PARSE) {
			printf("Parsing deck failed. Exiting...\n");
			return EXIT_FAILURE;
		}
	}

	// Create deck of card numbers drawn or not.
	cards = (int*)malloc(sizeof(int)*deck_size);
	memset(cards, CARD_IN_DECK, sizeof(int)*deck_size);

	srand((unsigned) time(&t));

	do {
		int the_card_index = rand() % (deck_size - cards_picked++);
		int plus_index = 0;
	
		for(int n = 0; n <= the_card_index; n++) {
			if(cards[n] == CARD_DRAWN) {
				the_card_index++;
			}
		}

		cards[the_card_index] = CARD_DRAWN;

		printf("Draw %d: %s", cards_picked, deck_list[the_card_index]);
	} while(fgets(str, sizeof(str), stdin) && cards_picked < deck_size);

	printf("\nEnding..\n");

	free(cards);
	free(deck_list);

	return EXIT_SUCCESS;
}
