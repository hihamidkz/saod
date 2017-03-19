#include "hashtab.h"
#include <stdio.h>
#include <stdlib.h>

#define HASHTAB_SIZE 128
#define HASHTAB_MUL 31

struct listnode {
	char *key;
	int value;
	
	struct listnode *next;
};

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		fprintf(stderr, "Error. Input the number of elements.\n");
		return 0;
	}
	FILE *stream = fopen("dal.txt", "r");
	char **words;
	int i, n = atoi(argv[1]);
	words = malloc(n * sizeof(char*));
	if (words == NULL) {
		fprintf(stderr, "Allocation error.");
		return 0;
	}
	for (i = 0; i < n; i++)
		words[i] = malloc(n * sizeof(char));
	for (i = 0; i < n; i++)
		fgets(words[i], n, stream);
	//for (i = 0; i < n; i++)
		//printf("%d %s\n", i, words[i]);
	fclose(stream);
	
	struct listnode *hashtab[HASHTAB_SIZE];
	int j, c[128], q[200000], s = 0;
	
	hashtab_init(hashtab);
	for (i = 1; i <= n; i++) {
		q[i - 1] = hashtab_add(hashtab, words[i - 1], i - 1);
		printf("%d: %d\n", i - 1, q[i - 1]);
	}
	for (i = 0; i < 128; i++)
		c[i] = 0;
	for (i = 0; i < n; i++)
		c[q[i]] += 1;
	for (i = 0; i < 128; i++) {
		if (c[i] > 1)
			s++;
	}
	printf("s = %d\n", s);
	return 0;
}
