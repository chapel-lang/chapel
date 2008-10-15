/* heap.c
 * Maintains a heap of records such that the key of each node is
 * larger or equal than those of its children
 *
 * The heap is used to maintain an ordering for the local buckets
 * (one bucket for each destination PE) that have pending updates.
 * The bucket with the largest number of pending updates is
 * always at the root of the heap.
 */

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "pool.h"

static Heap_Record_Ptr *heap; /* heap of records */
static int heapNodes;         /* number of records in heap */
static int *IndexToHeapNode;  /* aux array that keeps mapping from record's indices to heap nodes */
static POOL *Heap_Pool;       /* memory for heap records */

#define LEFT(x)     (((x) << 1) + 1)
#define RIGHT(x)    (((x) << 1) + 2)
#define PARENT(x)   (((x)-1) >> 1)
#define MAP_INDEX_TO_HEAP_NODE(node) (IndexToHeapNode[heap[(node)]->index] = (node))


void HPCC_ra_Heap_Init (int size)
{
  int i;

  heap = (Heap_Record_Ptr *) malloc (size * sizeof (Heap_Record_Ptr));
  heapNodes = 0;

  IndexToHeapNode = (int *) malloc (size * sizeof (int));
  for (i = 0; i < size; i ++) {
    IndexToHeapNode[i] = NOT_A_NODE;
  }

  /* initialize memory pool for heap nodes */
  Heap_Pool = HPCC_PoolInit (size, sizeof(Heap_Record));

}


void HPCC_ra_Heap_Insert (int index, int key)
{

  Heap_Record_Ptr newNode;
  int node, parent;

  newNode = (Heap_Record*) HPCC_PoolGetObj(Heap_Pool);
  newNode->index  = index;
  newNode->key = key;

  node = heapNodes;
  parent = PARENT(node);
  heapNodes ++;
  while (node != 0 && key > heap[parent]->key) {
    heap[node] = heap[parent];
    MAP_INDEX_TO_HEAP_NODE(node);
    node = parent;
    parent = PARENT(node);
  }

  heap[node] = newNode;
  IndexToHeapNode[index] = node;

}

void HPCC_ra_Heap_IncrementKey (int index, int key)
{

  int node;

  int parent;
  int child;
  int done;
  Heap_Record_Ptr tmp;

  node = IndexToHeapNode[index];
  if (node != NOT_A_NODE) {
    heap[node]->key = heap[node]->key + 1;

    /* _ra_Heapify (node); */
    done = 0;
    child = node;
    while (!done && child > 0) {
      parent = PARENT(child);
      if (parent >= 0 && heap[parent]->key < heap[child]->key) {
	tmp = heap[child];
	heap[child] = heap[parent];
	MAP_INDEX_TO_HEAP_NODE(child);
	heap[parent] = tmp;
	MAP_INDEX_TO_HEAP_NODE(parent);
	child = parent;
      }
      else done = 1;
    }
    /* end  _ra_Heapify (node); */

  }

}


void HPCC_ra_Heap_ExtractMax (int *index, int *key)
{
  Heap_Record_Ptr nodePtr;
  int parent, child;

  nodePtr = heap[HEAP_ROOT];
  *index = nodePtr->index;
  *key = nodePtr->key;
  HPCC_PoolReturnObj(Heap_Pool, nodePtr);

  heapNodes --;
  nodePtr = heap[heapNodes];
  parent = HEAP_ROOT;
  child = LEFT(parent);
  while (child <= heapNodes) {
    if (child < heapNodes && heap[child]->key < heap[child+1]->key)
      child ++;
    if (nodePtr->key >= heap[child]->key)
      break;
    heap[parent] = heap[child];
    MAP_INDEX_TO_HEAP_NODE(parent);
    parent = child;
    child  = LEFT(child);
  }

  heap[parent] = nodePtr;
  MAP_INDEX_TO_HEAP_NODE(parent);
}

void HPCC_ra_Heapify(int node)
{
  /* assumes that the key of a given node can only be increased */

  int parent;
  int child;
  int done;
  Heap_Record_Ptr tmp;

  done = 0;
  child = node;
  while (!done && child > 0) {
    parent = PARENT(child);
    if (parent >= 0 && heap[parent]->key < heap[child]->key) {
      tmp = heap[child];
      heap[child] = heap[parent];
      MAP_INDEX_TO_HEAP_NODE(child);
      heap[parent] = tmp;
      MAP_INDEX_TO_HEAP_NODE(parent);
      child = parent;
    }
    else done = 1;
  }
}


void HPCC_ra_Heapify_r(int node)
{
  /* assumes that the key of a given entry can only be increased */

  int parent;
  int child;
  Heap_Record_Ptr tmp;

  if (node > 0) {
    child = node;
    parent = PARENT(child);
    if (parent >= 0 && heap[parent]->key < heap[child]->key) {
      tmp = heap[child];
      heap[child] = heap[parent];
      MAP_INDEX_TO_HEAP_NODE(child);
      heap[parent] = tmp;
      MAP_INDEX_TO_HEAP_NODE(parent);
      child = parent;
      HPCC_ra_Heapify(child);
    }
  }
}


void HPCC_ra_Heap_Free() {

  HPCC_PoolFree(Heap_Pool);

  free(Heap_Pool);

  free(IndexToHeapNode);

  free(heap);

}
