#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

int main()
{
	struct trie *root;

	root = trie_insert(NULL, "bars", "60");
	root = trie_insert(root, "baribal", "100");
	root = trie_insert(root, "kit", "3000");
	root = trie_insert(root, "lev", "500");
	trie_print(root, 0);
	return 0;
}
