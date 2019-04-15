#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BINOM_COEFF_NOT_DEF 0xFFF0
#define DECK_SIZE 40
#define LAND_NUM_MIN 16
#define LAND_NUM_MAX 19
#define DRAWN_MAX 14


double factorial(int d) {
	double result = (double)d;

	if(d <= 1) { return 1; }

	d--;
	for(; d > 0; d--) {
		result = result * d;
	}
	return result;
}

double binomial_coefficient(int j, int k, int* failure_flag) {
	if(k > j) { printf("\t\t%d > %d...\n", j, k); *failure_flag = BINOM_COEFF_NOT_DEF; return (double)*failure_flag; }
	return (double) factorial(j) / (factorial(k) * factorial(j-k));
}

int main(int argc, char** argv) {
	char str[5];
	int deck_size = DECK_SIZE;
	int land_num_min = LAND_NUM_MIN;
	int land_num_max = LAND_NUM_MAX;
	int drawn_max = DRAWN_MAX;

	if(argc > 1) {
		deck_size = atoi(argv[1]);
		land_num_min = atoi(argv[2]);
		land_num_max = atoi(argv[3]);
		drawn_max = atoi(argv[4]);
	}

	printf("%d Cards; %d Draws --\n", deck_size, drawn_max);
	for(int land_num = land_num_min; land_num <= land_num_max; land_num++) {
		double total_odds = 0;
		int not_lands = deck_size - land_num;

		printf("%d Lands --\n", land_num);
		for(int drawn = 0; drawn <= drawn_max; drawn++) {
			int not_lands_drawn = drawn_max - drawn;
			double this_odds = 0.0;
			int binom_coeff_failure_flag = 0;

			if(drawn > 0)
				this_odds = 100 * binomial_coefficient(land_num, drawn, &binom_coeff_failure_flag) * binomial_coefficient(not_lands, not_lands_drawn, &binom_coeff_failure_flag) / 
						binomial_coefficient(deck_size, drawn_max, &binom_coeff_failure_flag);
			else
				this_odds = 100 * (double)((factorial(deck_size - land_num) * factorial(deck_size - drawn_max) / 
							(factorial(deck_size) * factorial(deck_size - land_num - drawn_max))));
			
			if(binom_coeff_failure_flag == BINOM_COEFF_NOT_DEF)
				break; 

			total_odds = total_odds + this_odds;
			
			printf("\t%d Lands Drawn %%%.4g.", drawn, this_odds);
			if(drawn < drawn_max) {
				printf(" \t-- (%%%.4g give %d or more).", 100-total_odds, drawn+1);
			}
			printf("\n");
		}
	}

	return EXIT_SUCCESS;
}
