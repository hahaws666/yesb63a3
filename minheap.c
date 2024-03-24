/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- to help designing your code
 *************************************************************************/

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap *heap, int maybeIdx)
{
       return (heap->size >= maybeIdx && maybeIdx != 0 && maybeIdx != NOTHING);
}

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap *heap, int nodeIndex)
{
       return heap->arr[nodeIndex].priority;
}

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap *heap, int index1, int index2)
{
       if (isValidIndex(heap, index1) && isValidIndex(heap, index2))
       {
              int temp1, temp2, id1, id2;
              id1 = heap->arr[index1].id;
              id2 = heap->arr[index2].id;
              temp1 = heap->arr[index1].priority;
              temp2 = heap->arr[index2].priority;
              heap->arr[index2].priority = temp1;
              heap->arr[index1].priority = temp2;
              heap->arr[index1].id = id2;
              heap->arr[index2].id = id1;
              heap->indexMap[id1] = index2;
              heap->indexMap[id2] = index1;
       }
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap *heap, int nodeIndex)
{
       int res = nodeIndex / 2;
       if (res == 0)
       {
              return NOTHING;
       }
       return res;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap *heap, int nodeIndex)
{
       int res = nodeIndex * 2;
       if (!isValidIndex(heap, res))
       {
              return NOTHING;
       }

       return res;
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap *heap, int nodeIndex)
{
       int res = nodeIndex * 2 + 1;
       if (!isValidIndex(heap, res))
       {
              return NOTHING;
       }

       return res;
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap *heap, int nodeIndex)
{
       int g = parentIdx(heap, nodeIndex);
       if (!isValidIndex(heap, g))
       {
              return;
       }

       if (heap->arr[g].priority > heap->arr[nodeIndex].priority)
       {
              swap(heap, g, nodeIndex);
              bubbleUp(heap, g);
       }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap *heap, int nodeindex)
{
       if ((!heap) || (!isValidIndex(heap, nodeindex)))
       {
              return;
       }

       int left_idx = leftIdx(heap, nodeindex);
       int right_idx = rightIdx(heap, nodeindex);
       if (left_idx == NOTHING && right_idx == NOTHING)
       {
              return;
       }
       if (left_idx != NOTHING)
       {
              if (priorityAt(heap, left_idx) < priorityAt(heap, nodeindex))
              {
                     if (right_idx != NOTHING)
                     {
                            if (priorityAt(heap, left_idx) < priorityAt(heap, right_idx))
                            {
                                   swap(heap, left_idx, nodeindex);
                                   bubbleDown(heap, left_idx);
                            }
                            else
                            {
                                   swap(heap, right_idx, nodeindex);
                                   bubbleDown(heap, right_idx);
                            }
                     }
                     else
                     {
                            swap(heap, left_idx, nodeindex);
                            bubbleDown(heap, left_idx);
                     }
              }
       }
       if (right_idx != NOTHING)
       {
              if (priorityAt(heap, right_idx) < priorityAt(heap, nodeindex))
              {
                     swap(heap, right_idx, nodeindex);
                     bubbleDown(heap, right_idx);
              }
       }
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap *heap, int nodeIndex)
{
       return heap->arr[nodeIndex];
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap *heap, int nodeIndex)
{
       return heap->arr[nodeIndex].id;
}

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap *heap, int id)
{
       return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/
/* Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode getMin(MinHeap *heap)
{
       return heap->arr[ROOT_INDEX];
}

/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap *heap, int id)
{
       return heap->arr[heap->indexMap[id]].priority;
}

/* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode extractMin(MinHeap *heap)
{
       HeapNode res;
       res.id = heap->arr[ROOT_INDEX].id;
       res.priority = heap->arr[ROOT_INDEX].priority;
       heap->indexMap[res.id] = NOTHING;
       swap(heap, ROOT_INDEX, heap->size);
       heap->arr[heap->size].id = NOTHING;
       heap->arr[heap->size].priority = NOTHING;
       heap->size--;
       heap->indexMap[res.id] = NOTHING;
       bubbleDown(heap, 1);
       return res;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap *heap, int priority, int id)
{

       if (heap->size >= heap->capacity || id < 0 || id >= heap->capacity)
       {
              return;
       }
       heap->size++;
       heap->indexMap[id] = heap->size;
       heap->arr[heap->size].id = id;
       heap->arr[heap->size].priority = priority;
       bubbleUp(heap, heap->size);
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap *heap, int id, int newPriority)
{

       int id_idx = heap->indexMap[id];
       if (heap->arr[id_idx].priority > newPriority)
       {
              heap->arr[id_idx].priority = newPriority;
              bubbleUp(heap, id_idx);
              return true;
       }
       return false;
}

/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap *newHeap(int capacity)
{
       MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
       if (heap == NULL)
       {
              fprintf(stderr, "Memory is not enough\n");
       }
       heap->size = 0;
       heap->capacity = capacity;
       heap->arr = (HeapNode *)malloc(sizeof(HeapNode) * (capacity + 1));
       heap->indexMap = (int *)malloc(sizeof(int) * (capacity));
       for (int i = 0; i < capacity; i++)
       {
              heap->indexMap[i] = NOTHING;
              heap->arr[i].id = NOTHING;
              heap->arr[i].priority = NOTHING;
       }
       heap->arr[capacity].id = NOTHING;
       heap->arr[capacity].priority = NOTHING;
       return heap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap *heap)
{
       free(heap->arr);
       free(heap->indexMap);
       free(heap);
}

/*********************************************************************
 ** Helper function provided
 *********************************************************************/
void printHeap(MinHeap *heap)
{
       printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
              heap->capacity);
       printf("index: priority [ID]\t ID: index\n");
       for (int i = 0; i < heap->capacity; i++)
              printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
                     indexOf(heap, i));
       printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
              idAt(heap, heap->capacity));
       printf("\n\n");
}

// int main(){
//        MinHeap *new = newHeap(4);
//        insert(new, 1, 2);
//        insert(new, 3, 0);
//        insert(new, 2, 1);
//        insert(new, 9, 3);
//        printHeap(new);
// }
