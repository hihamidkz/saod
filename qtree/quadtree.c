#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

struct point {
	int x;
	int y;
};

struct quadtree {
	struct point *key;
	char *value;
	
	struct quadtree *northwest;
	struct quadtree *northeast;
	struct quadtree *southwest;
	struct quadtree *southeast;
	struct quadtree *parent;
};

struct quadtree *quadtree_create()
{
	struct quadtree *qtree;
	
	qtree = malloc(sizeof(*qtree));
	qtree->key = malloc(sizeof(*qtree->key));
	if (qtree == NULL || qtree->key == NULL)
		return NULL;
	
	qtree->value = NULL;
	qtree->northwest = NULL;
	qtree->northeast = NULL;
	qtree->southwest = NULL;
	qtree->southeast = NULL;
	qtree->parent = NULL;
	return qtree;
}

struct quadtree *quadtree_insert(struct quadtree *qtree, int x, int y, char *value)
{
	struct quadtree *node, *parent;
	
	if (qtree == NULL) {
		qtree = quadtree_create();
		qtree->key->x = x;
		qtree->key->y = y;
		qtree->value = value;
		return qtree;
	}
	
	node = qtree;
	for (parent = node; node != NULL; ) {
		parent = node;
		if (x < node->key->x && y > node->key->y)
			node = node->northwest;
		else if (x < node->key->x && y < node->key->y)
			node = node->southwest;
		else if (x > node->key->x && y > node->key->y)
			node = node->northeast;
		else
			node = node->southeast;
	}
	node = quadtree_create();
	node->key->x = x;
	node->key->y = y;
	node->value = value;
	if (x < parent->key->x && y > parent->key->y)
		parent->northwest = node;
	else if (x < parent->key->x && y < parent->key->y)
		parent->southwest = node;
	else if (x > parent->key->x && y > parent->key->y)
		parent->northeast = node;
	else
		parent->southeast = node;
	node->parent = parent;
	return qtree;
}

struct quadtree *quadtree_lookup(struct quadtree *q, int x, int y)
{
	struct quadtree *node;
	
	for (node = q; node != NULL; ) {
		if (node->key->x == x && node->key->y == y)
			break;
		if (node->key->x < x) {
			if (node->key->y < y)
				node = node->northeast;
			else
				node = node->southeast;
		} else {
			if (node->key->y < y)
				node = node->northwest;
			else
				node = node->southwest;
		}
	}
	
	return node;
}

struct quadtree *find_nearest_point(struct quadtree *q, int x, int y)
{
	struct quadtree *node, *best, **temp;
	int i, dist, best_dist;
	
	temp = malloc(9 * sizeof(struct quadtree *));
	
	best_dist = INT_MAX;
	node = quadtree_lookup(q, x, y);
	
	if (node == NULL)
		return NULL;
	
	temp[0] = node->northeast;
	temp[1] = node->northwest;
	temp[2] = node->southeast;
	temp[3] = node->southwest;
	if (node->parent != NULL) {
		temp[4] = node->parent;
		temp[5] = node->parent->northeast;
		temp[6] = node->parent->northwest;
		temp[7] = node->parent->southeast;
		temp[8] = node->parent->southwest;
	}
	
	for (i = 0; i < 9; i++) {
		if (temp[i] == NULL)
			continue;
		dist = sqrt(pow(temp[i]->key->x - x, 2) + pow(temp[i]->key->y - y, 2));
		if (dist < best_dist && dist != 0) {
			best_dist = dist;
			best = temp[i];
		}
	}
	
	return best;
}

struct quadtree *deepest_northeast_point(struct quadtree *q)
{
	if (q == NULL)
		return NULL;

	struct quadtree *node = q;
	
	while (node->northeast != NULL)
		node = node->northeast;
	
	return node;
}

struct quadtree *deepest_southwest_point(struct quadtree *q)
{
	if (q == NULL)
		return NULL;

	struct quadtree *node = q;
	
	while (node->southwest != NULL)
		node = node->southwest;
		
	return node;
}

struct quadtree **query_range(struct quadtree *node, struct quadtree **nodes, struct point *b1, struct point *b2, int *idx)
{	
	if (node == NULL)
		return nodes;
	
	if (nodes == NULL)
		nodes = calloc(10, sizeof(struct quadtree *));
	
	if (node->key->x >= b1->x && node->key->y >= b1->y) {
		if (node->key->x <= b2->x && node->key->y <= b2->y) {
			nodes[*idx] = node;
			*idx += 1;
		}
	}
	query_range(node->northwest, nodes, b1, b2, idx);
	query_range(node->northeast, nodes, b1, b2, idx);
	query_range(node->southwest, nodes, b1, b2, idx);
	query_range(node->southeast, nodes, b1, b2, idx);
	
	return nodes;
}

void quadtree_print(struct quadtree *q, int level)
{
	if (q == NULL)
		return;
		
	int i;
	
	for (i = 0; i < level; i++)
		printf("   ");

	printf("%s (%d, %d)", q->value, q->key->x, q->key->y);
	if (q->parent == NULL)
		printf("  (root)\n");
	else if (q == q->parent->northwest)
		printf("  (NW)\n");
	else if (q == q->parent->northeast)
		printf("  (NE)\n");
	else if (q == q->parent->southwest)
		printf("  (SW)\n");
	else
		printf("  (SE)\n");
	level++;
	quadtree_print(q->northwest, level);
	quadtree_print(q->northeast, level);
	quadtree_print(q->southwest, level);
	quadtree_print(q->southeast, level);
}

int main()
{
	int i;
	struct quadtree *q = NULL;
	
	q = quadtree_insert(q, 5, 45, "Denver");
	q = quadtree_insert(q, 60, 77, "Toronto");
	q = quadtree_insert(q, 80, 65, "Buffalo");
	q = quadtree_insert(q, 35, 40, "Chicago");
	q = quadtree_insert(q, 25, 35, "Omaha");
	q = quadtree_insert(q, 50, 10, "Mobile");
	q = quadtree_insert(q, 85, 15, "Atlanta");
	q = quadtree_insert(q, 90, 5, "Miami");
	quadtree_print(q, 0);
	printf("Lookup (35, 42): %s\n", quadtree_lookup(q, 35, 40)->value);
	printf("Find nearest neighbor to point (5, 45): %s\n", find_nearest_point(q, 50, 10)->value);
	struct quadtree **qt = NULL;
	struct point a = {25, 10};
	struct point b = {60, 65};
	int j = 0;
	qt = query_range(q, qt, &a, &b, &j);
	
	for (i = 0; ; i++) {
		if (qt[i] == NULL)
			break;
		printf("%s\n", qt[i]->value);
	}
	return 0;
}




