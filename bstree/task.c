#include <stdio.h>
#include <stdlib.h>
#include "bstree.h"
#include "hashtab.h"
#include <sys/time.h>

struct bstree {
	char *key;
	int value;
	
	struct bstree *left;
	struct bstree *right;
};

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max)
{
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		fprintf(stderr, "Error.\n");
		return 0;
	}
	FILE *stream = fopen("war/war.txt", "r");
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
		//printf("%s\n", words[i]);
	fclose(stream);
	struct bstree *tree, *node;
	
	tree = bstree_create(words[0], 0);
	/*for (i = 2; i <= n; i++) {
		bstree_add(tree, words[i - 1], i - 1);
		if (i % 10000 == 0) {
			char *w = words[getrand(0, i - 1)];
			double t = wtime();
			node = bstree_lookup(tree, w);
			t = wtime() - t;
			printf("Binary search tree: n = %d, t = %.6f\n", i, t);
		}
	}*/
	char *w;
	free(words);
	return 0;
}
