#ifndef FIBHEAP_H
#define FIBHEAP_H

#define FALSE 0
#define TRUE 1

struct heapnode {
	int key;
	char *value;

	struct heapnode *parent;
	struct heapnode *child;
	struct heapnode *left;
	struct heapnode *right;

	int degree;
	int mark;
};

struct fibheap {
	int nnodes;

	struct heapnode *min;
	struct heapnode *nodes;
};

struct fibheap *FibHeapCreate();
void AddNodeToRootList(struct heapnode *node, struct heapnode *minnode);
struct fibheap *FibHeapInsert(struct fibheap *heap, int key, char *value);
struct heapnode *FibHeapMin(struct fibheap *heap);
struct fibheap *FibHeapUnion(struct fibheap *heap1, struct fibheap *heap2);
struct heapnode *FibHeapLinkLists(struct heapnode *heap1, struct heapnode *heap2);
void FibHeapLink(struct heapnode *y, struct heapnode *x);
struct heapnode *FibHeapDeleteMin(struct fibheap *heap);
struct fibheap *FibHeapConsolidate(struct fibheap *heap);
void FibHeapDecreaseKey(struct fibheap *heap, struct heapnode *node, int newkey);
void FibHeapCascadingCut(struct fibheap *heap, struct heapnode *y);
void FibHeapDelete(struct fibheap *heap, struct heapnode *x);
void FibHeapCut(struct fibheap *heap, struct heapnode *x, struct heapnode *y);

#endif
