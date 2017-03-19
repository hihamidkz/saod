#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int main(int argc, char **argv)
{
	struct rbtree *tree = NULL;

	int i;
	
	if (argc < 2) {
		fprintf(stderr, "Input keys\n");
		return -1;
	}
	for (i = 1; i < argc; ++i)
		tree = rbtree_add(tree, atoi(argv[i]), argv[i + 1]);
	rbtree_print_dfs(tree, 0);
	tree = rbtree_delete(tree, 6);
	printf("\n");
	rbtree_print_dfs(tree, 0);
	//printf("\n%d\n", tree->key);
	return 0;
}
