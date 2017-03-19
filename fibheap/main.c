#include <stdio.h>

#include "fibheap.h"

int main()
{
	struct fibheap *heap = NULL;

	heap = FibHeapCreate();
	heap = FibHeapInsert(heap, 5, "5");
	heap = FibHeapInsert(heap, 10, "10");
	heap = FibHeapInsert(heap, 3, "3");
	heap = FibHeapInsert(heap, 4, "3");
	heap = FibHeapInsert(heap, 8, "3");
	heap = FibHeapInsert(heap, 20, "3");
	heap = FibHeapInsert(heap, 22, "3");
	heap = FibHeapInsert(heap, 100, "3");
	struct heapnode *min = FibHeapDeleteMin(heap);
	printf("Min key: %d\n", min->key);
	FibHeapDecreaseKey(heap, heap->min, 1);
	printf("Decrease minimum key from %d to 1\n", min->key);
	FibHeapDelete(heap, heap->min);
	printf("Delete minimum key. New minimum key: %d\n", heap->min->key);
	return 0;
}
