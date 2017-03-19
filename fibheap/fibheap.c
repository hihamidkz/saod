#include "fibheap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct fibheap *FibHeapCreate()
{
	struct fibheap *heap;

	heap = (struct fibheap *)malloc(sizeof(struct fibheap));
	if (heap == NULL)
		return NULL;

	heap->nnodes = 0;
	heap->min = NULL;
	heap->nodes = NULL;
	return heap;
}

void AddNodeToRootList(struct heapnode *node, struct heapnode *minnode)
{
	struct heapnode *lnode;

	if (minnode == NULL)
		return;

	if (minnode->left == minnode) {
		minnode->left = node;
		minnode->right = node;
		node->right = minnode;
		node->left = minnode;
	} else {
		lnode = minnode->left;
		minnode->left = node;
		node->right = minnode;
		node->left = lnode;
		lnode->right = node;
	}
}

struct fibheap *FibHeapInsert(struct fibheap *heap, int key, char *value)
{
	struct heapnode *node;

	node = (struct heapnode *)malloc(sizeof(struct heapnode));
	if (node == NULL)
		return NULL;

	node->key = key;
	node->value = value;
	node->degree = 0;
	node->mark = FALSE;
	node->parent = NULL;
	node->child = NULL;
	node->left = node;
	node->right = node;
	AddNodeToRootList(node, heap->min);
	if (heap->min == NULL || node->key < heap->min->key)
		heap->min = node;
	heap->nnodes++;
	return heap;
}


struct heapnode *FibHeapMin(struct fibheap *heap)
{
	return heap->min;
}

struct fibheap *FibHeapUnion(struct fibheap *heap1, struct fibheap *heap2)
{
	struct fibheap *heap;

	heap = FibHeapCreate();
	heap->min = heap1->min;
	heap->nodes = FibHeapLinkLists(heap1->min, heap2->min);
	if ((heap1->min == NULL) || (heap2->min != NULL && heap2->min->key < heap1->min->key))
		heap->min = heap2->min;
	heap->nnodes = heap1->nnodes + heap2->nnodes;
	//free(heap1);
	//free(heap2);
	return heap;
}

struct heapnode *FibHeapLinkLists(struct heapnode *heap1, struct heapnode *heap2)
{
	if (heap1 == NULL || heap2 == NULL)
		return NULL;

	struct heapnode *left1 = heap1->left;
	struct heapnode *left2 = heap2->left;

	left1->right = heap2;
	heap2->left = left1;
	heap1->left = left2;
	left2->right = heap1;

	return heap1;
}

double log_2(double a)
{
	return log(a) / log(2.0);
}

void FibHeapLink(struct heapnode *y, struct heapnode *x)
{
	x->degree++;
	y->left->right = y->right;
	y->right->left = y->left;
	if (x->child != NULL) {
		x->child->left->right = y;
		y->left = x->child->left;
		x->child->left = y;
		y->right = x->child;
	} else {
		x->child = y;
		y->right = y;
		y->left = y;
	}
	y->parent = x;
	y->mark = FALSE;
}

struct heapnode *FibHeapDeleteMin(struct fibheap *heap)
{
	struct heapnode *z = heap->min;
	struct heapnode *node = NULL;
	struct heapnode *temp, *p = NULL;
	struct heapnode *tmp = z;

	if (z == NULL)
		return NULL;
	if (z->child != NULL)
		node = z->child;
	if (node != NULL) {
		temp = node;
		while (p != temp) {
			p = node->right;
			tmp->left->right = node;
			node->right = tmp;
			node->left = tmp->left;
			tmp->left = node;
			if (node->key < tmp->key)
				tmp = node;
			node->parent = NULL;
			node = p;
		}
	}
	z->left->right = z->right;
	z->right->left = z->left;
	if (z == z->right)
		heap->min = NULL;
	else {
		heap->min = z->right;
		FibHeapConsolidate(heap);
	}
	heap->nnodes--;
	return z;
}

struct fibheap *FibHeapConsolidate(struct fibheap *heap)
{
	int i;
	int n;
	int rootsize = 0;

	struct heapnode **A, **rootlist;
	struct heapnode *x;
	struct heapnode *y;
	struct heapnode *next;
	struct heapnode *tmp;
	struct heapnode *w;
	n = (int)floor(log_2(heap->nnodes));
	A = (struct heapnode **)calloc(n + 2, sizeof(struct heapnode*));
	w = heap->min;
	next = w;
	while (tmp != w) {
		next = next->right;
		tmp = next;
		rootsize++;
	}
	rootlist = malloc(rootsize * sizeof(struct heapnode *));
	for (i = 0; i < rootsize; i++)
	{
		rootlist[i] = next;
		next = next->right;
	}
	
	for (i = 0; i < rootsize; i++) {
		w = rootlist[i];
		x = w;
		int d = x->degree;
		while (A[d] != NULL) {
			y = A[d];
			if (x->key > y->key) {
				struct heapnode *temp = x;
				x = y;
				y = temp;
			}
			FibHeapLink(y, x);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
	}
	free(rootlist);
	heap->min = NULL;
	for (i = 0; i < n + 2; i++) {
		if (A[i] != NULL) {
			if (heap->min == NULL)
				heap->min = A[i];
			else {
				heap->min->left->right = A[i];
				A[i]->left = heap->min->left;
				heap->min->left = A[i];
				A[i]->right = heap->min;
				if (A[i]->key < heap->min->key)
					heap->min = A[i];
			}
		}
	}
	free(A);
	return heap;
}
	
void FibHeapDecreaseKey(struct fibheap *heap, struct heapnode *node, int newkey)
{
	if (newkey > node->key)
		return;
	
	node->key = newkey;
	struct heapnode *y = node->parent;
	if (y != NULL && node->key < y->key) {
		FibHeapCut(heap, node, y);
		FibHeapCascadingCut(heap, y);
	}
	if (node->key < heap->min->key)
		heap->min = node;
}

void FibHeapCut(struct fibheap *heap, struct heapnode *x, struct heapnode *y)
{
	x->left->right = x->right;
	x->right->left = x->left;
	y->degree--;
	AddNodeToRootList(x, heap->min);
	x->parent = NULL;
	x->mark = FALSE;
}

void FibHeapCascadingCut(struct fibheap *heap, struct heapnode *y)
{
	struct heapnode *z = y->parent;
	if (z == NULL)
		return;
	if (y->mark == FALSE)
		y->mark = TRUE;
	else {
		FibHeapCut(heap, y, z);
		FibHeapCascadingCut(heap, z);
	}
}

void FibHeapDelete(struct fibheap *heap, struct heapnode *x)
{
	FibHeapDecreaseKey(heap, x, INT_MAX);
	FibHeapDeleteMin(heap);
}




