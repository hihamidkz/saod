#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

struct rbtree EmptyNode = {0, 0, BLACK, NULL, NULL, NULL};
struct rbtree *NullNode = &EmptyNode;
struct rbtree *rbtree_right_rotate(struct rbtree *root, struct rbtree *node);
struct rbtree *rbtree_left_rotate(struct rbtree *root, struct rbtree *node);

struct rbtree *rbtree_fixup_add(struct rbtree *root, struct rbtree *node)
{
	struct rbtree *uncle;

	while (node != root && node->parent->color == RED) {
		if (node->parent == node->parent->parent->left) {
			uncle = node->parent->parent->right;
			if (uncle->color == RED) {
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			} else {
				if (node == node->parent->right) {
					node = node->parent;
					root = rbtree_left_rotate(root, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				root = rbtree_right_rotate(root, node->parent->parent);
			}
		} else {
			uncle = node->parent->parent->left;
			if (uncle->color == RED) {
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			} else {
				if (node == node->parent->left) {
					node = node->parent;
					root = rbtree_right_rotate(root, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				root = rbtree_left_rotate(root, node->parent->parent);
			}
		}
	}
	root->color = BLACK;
	return root;
}

struct rbtree *rbtree_left_rotate(struct rbtree *root, struct rbtree *node)
{
	struct rbtree *right = node->right;

	node->right = right->left;
	if (right->left != NullNode)
		right->left->parent = node;

	if (right != NullNode)
		right->parent = node->parent;
	if (node->parent != NullNode) {
		if (node == node->parent->left)
			node->parent->left = right;
		else
			node->parent->right = right;
	} else {
		root = right;
	}

	right->left = node;
	if (node!= NullNode)
		node->parent = right;
	return root;
}

struct rbtree *rbtree_right_rotate(struct rbtree *root, struct rbtree *node)
{
	struct rbtree *left = node->left;

	node->left = left->right;
	if (left->right != NullNode) 
		left->right->parent = node;
	if (left != NullNode)
		left->parent = node->parent;
	if (node->parent != NullNode) {
		if (node == node->parent->right)
			node->parent->right = left;
		else
			node->parent->left = left;
	} else {
		root = left;
	}
	
	left->right = node;
	if (node != NullNode) 
		node->parent = left;
	return root;
}

struct rbtree *rbtree_add(struct rbtree *root, int key, char *value)
{
	struct rbtree *node, *parent = NullNode;

	for (node = root; node != NullNode && node != NULL; ) {
		parent = node;
		if (key < node->key)
			node = node->left;
		else if (key > node->key)
			node = node->right;
		else
			return root;
	}
	node = malloc(sizeof(*node));
	if (node == NULL)
		return NULL;

	node->key = key;
	node->value = value;
	node->color = RED;
	node->parent = parent;
	node->left = NullNode;
	node->right = NullNode;
	
	if (parent != NullNode) {
		if (key < parent->key)
			parent->left = node;
		else
			parent->right = node;
	} else {
		root = node;
	}
	return rbtree_fixup_add(root, node);
}

struct rbtree *rbtree_lookup(struct rbtree *root, int key)
{
	while (root != NULL) {
		if (key == root->key)
			return root;
		else if (key < root->key)
			root = root->left;
		else
			root = root->right;
	}
	return root;
}

struct rbtree *rbtree_min(struct rbtree *root)
{
	if (root == NULL)
		return NULL;
	while (root->left != NullNode)
		root = root->left;
	return root;
}

struct rbtree *rbtree_max(struct rbtree *root)
{
	if (root == NULL)
		return NULL;
	while (root->right != NullNode)
		root = root->right;
	return root;
}

struct rbtree *rbtree_transplant(struct rbtree *root, struct rbtree *u, struct rbtree *v)
{
	if (u->parent == NullNode)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
	return root;
}

struct rbtree *rbtree_delete_fixup(struct rbtree *root, struct rbtree *node)
{
	struct rbtree *brother;
	
	while (node != root && node->color == BLACK) {
		if (node == node->parent->left) {
			brother = node->parent->right;
			if (brother->color == RED) {
				brother->color = BLACK;
				node->parent->color = RED;
				rbtree_left_rotate(root, node->parent);
				brother = node->parent->right;
			}
			if (brother->left->color == BLACK && brother->right->color == BLACK) {
				brother->color = RED;
				node = node->parent;
			} else {
				if (brother->right->color == BLACK) {
					brother->left->color = BLACK;
					brother->color = RED;
					rbtree_right_rotate(root, brother);
					brother = node->parent->right;
				}
				brother->color = node->parent->color;
				node->parent->color = BLACK;
				brother->right->color = BLACK;
				rbtree_left_rotate(root, node->parent);
				node = root;
			} 
		} else {
				brother = node->parent->left;
				if (brother->color == RED) {
					brother->color = BLACK;
					node->parent->color = RED;
					rbtree_right_rotate(root, node->parent);
					brother = node->parent->left;
				}
				if (brother->right->color == BLACK && brother->left->color == BLACK) {
					brother->color = RED;
					node = node->parent;
				} else {
					if (brother->left->color == BLACK) {
						brother->right->color = BLACK;
						brother->color = RED;
						rbtree_left_rotate(root, brother);
						brother = node->parent->left;
					}
					brother->color = node->parent->color;
					node->parent->color = BLACK;
					brother->left->color = BLACK;
					rbtree_right_rotate(root, node->parent);
					node = root;
				}
		}
	}
	node->color = BLACK;
	return root;
}

struct rbtree *rbtree_delete(struct rbtree *root, int key)
{
	struct rbtree *node;
	struct rbtree *y;
	struct rbtree *x;

	int y_color;

	node = rbtree_lookup(root, key);
	y = node;
	y_color = y->color;
	
	if (node->left == NullNode) {
		x = node->right;
		root = rbtree_transplant(root, node, node->right);
	}
	else if (node->right == NullNode) {
		x = node->left;
		root = rbtree_transplant(root, node, node->left);
	}
	else {
		y = rbtree_min(node->right);
		y_color = y->color;
		x = y->right;
		if (y->parent == node)
			x->parent = y;
		else {
			root = rbtree_transplant(root, y, y->right);
			y->right = node->right;
			y->right->parent = y;
		}
		root = rbtree_transplant(root, node, y);
		y->left = node->left;
		y->left->parent = y;
		y->color = node->color;
	}
	if (y_color == BLACK)
		root = rbtree_delete_fixup(root, x);
	return root;
}

void rbtree_print_dfs(struct rbtree *tree, int level)
{
	int i;

	if (tree == NULL)
		return;
	for (i = 0; i < level; i++)
		printf("  ");
	if (tree->color == RED)
		printf("\033[31m%d\033[0m\n", tree->key);
	else
		printf("%d\n", tree->key);
	rbtree_print_dfs(tree->left, level + 1);
	rbtree_print_dfs(tree->right, level + 1);
}
