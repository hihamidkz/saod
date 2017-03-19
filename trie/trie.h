#ifndef TRIE_H
#define TRIE_H

struct trie {
	char *value;
	char ch;
	struct trie *sibling;
	struct trie *child;
};

struct trie *trie_create();
struct trie *trie_insert(struct trie *root, char *key, char *value);
char *trie_lookup(struct trie *root, char *key);
struct trie *trie_delete(struct trie *root, char *key);
struct trie *trie_delete_dfs(struct trie *root, struct trie *parent, char *key, int *found);
void trie_print(struct trie *root, int level);

#endif
