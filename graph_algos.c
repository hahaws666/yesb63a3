/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records
{
  int numVertices;   // total number of vertices in the graph
                     // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap *heap;     // priority queue
  bool *finished;    // finished[id] is true iff vertex id is finished
                     //   i.e. no longer in the PQ
  int *predecessors; // predecessors[id] is the predecessor of vertex id
  Edge *tree;        // keeps edges for the resulting tree
  int numTreeEdges;  // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap *initHeap(Graph *graph, int startVertex)
{
  MinHeap *res = newHeap(graph->numVertices);
  for (int i = 0; i < graph->numVertices; i++)
  {
    insert(res, INT_MAX, graph->vertices[i]->id);
  }
  return res;
}

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records *initRecords(Graph *graph, int startVertex)
{
  Records *res = malloc(sizeof(Records));
  res->numTreeEdges = 0;
  res->tree = malloc(sizeof(Edge) * (graph->numVertices));
  res->heap = initHeap(graph, startVertex);
  res->numVertices = graph->numVertices;
  res->finished = malloc(sizeof(bool) * (graph->numVertices));
  res->predecessors = malloc(sizeof(int) * (graph->numVertices));
  for (int i = 0; i < graph->numVertices; i++)
  {
    res->finished[i] = false;
    res->predecessors[i] = NOTHING;
    res->tree[i].fromVertex = NOTHING;
    res->tree[i].toVertex = NOTHING;
    res->tree[i].weight = NOTHING;
  }
  decreasePriority(res->heap, startVertex, 0);
  return res;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap *heap)
{
  return heap->size == 0;
}

/* Prints the status of all current algorithm data: good for debugging. */
// void printRecords(Records *records)
// {
//   printEdge(records->tree);
//   printHeap(records->heap);
//   printf("numvex: %d\n", records->numVertices);
//   printf("numedge: %d\n", records->numVertices);
// }

/* Add a new edge to records at index ind. */
void addTreeEdge(Records *records, int ind, int fromVertex, int toVertex,
                 int weight)
{
  records->numTreeEdges++;
  records->tree[ind].fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight;
}

// 狸猫换太子
void addTreeeEdge(Edge *edge, int ind, int fromVertex, int toVertex,
                  int weight)
{
  edge[ind].fromVertex = toVertex;
  edge[ind].toVertex = fromVertex;
  edge[ind].weight = weight;
}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList *makePath(Edge *distTree, int vertex, int startVertex);

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records *records)
{
  if (records == NULL)
    return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++)
    printEdge(&records->tree[i]);

  printf("... done.\n");
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge *getMSTprim(Graph *graph, int startVertex)
{
  Edge *mstEdges = malloc(sizeof(Edge) * (graph->numVertices - 1));
  Records *records = initRecords(graph, startVertex);

  while (!isEmpty(records->heap))
  {
    HeapNode minNode = extractMin(records->heap);
    int u = minNode.id;
    records->finished[u] = true;
    if (u != startVertex)
    {
      addTreeeEdge(mstEdges, records->numTreeEdges, records->predecessors[u], u, minNode.priority);
      addTreeEdge(records, records->numTreeEdges, records->predecessors[u], u, minNode.priority);
    }
    EdgeList *adjList = graph->vertices[u]->adjList;
    while (adjList != NULL)
    {
      int v = adjList->edge->toVertex;
      int weight = adjList->edge->weight;

      if (!records->finished[v] && decreasePriority(records->heap, v, weight))
      {
        records->predecessors[v] = u;
      }
      adjList = adjList->next;
    }
  }
  // Clean
  deleteHeap(records->heap);
  free(records->tree);
  free(records->finished);
  free(records->predecessors);
  free(records);

  return mstEdges;
}

/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge *getDistanceTreeDijkstra(Graph *graph, int startVertex)
{
  Edge *distanceEdges = malloc(sizeof(Edge) * (graph->numVertices));
  Records *records = initRecords(graph, startVertex);
  addTreeeEdge(distanceEdges, 0, startVertex, startVertex, 0);
  while (!isEmpty(records->heap))
  {
    HeapNode minNode = extractMin(records->heap);
    int u = minNode.id;
    int u_d = minNode.priority;
    records->finished[u] = true;
    if (u != startVertex)
    {
      addTreeeEdge(distanceEdges, records->numTreeEdges + 1, records->predecessors[u], u, minNode.priority);
      addTreeEdge(records, records->numTreeEdges, records->predecessors[u], u, minNode.priority);
    }
    EdgeList *adjList = graph->vertices[u]->adjList;
    while (adjList != NULL)
    {
      int v = adjList->edge->toVertex;
      int weight = adjList->edge->weight;
      if (!records->finished[v] && decreasePriority(records->heap, v, weight + u_d))
      {
        records->predecessors[v] = u;
      }
      adjList = adjList->next;
    }
  }
  // just for test
  // printRecords(records);
  deleteHeap(records->heap);
  free(records->tree);
  free(records->finished);
  free(records->predecessors);
  free(records);
  return distanceEdges;
}


Edge* cloneEdge(Edge* originalEdge) {
    if (originalEdge == NULL) return NULL;

    // Allocate memory for the new edge and copy the data
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    if (newEdge != NULL) {
        newEdge->fromVertex = originalEdge->fromVertex;
        newEdge->toVertex = originalEdge->toVertex;
        newEdge->weight = originalEdge->weight;
    }
    return newEdge;
}

EdgeList* cloneEdgeList(EdgeList* originalList) {
    if (originalList == NULL) return NULL;

    // Allocate memory for the new list node
    EdgeList* newList = (EdgeList*)malloc(sizeof(EdgeList));
    if (newList == NULL) return NULL;

    // Clone the edge
    newList->edge = cloneEdge(originalList->edge);

    // Recursively clone the rest of the list
    newList->next = cloneEdgeList(originalList->next);

    return newList;
}


/* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the list of edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
EdgeList **getShortestPaths(Edge *distTree, int numVertices, int startVertex)
{
  EdgeList **paths = (EdgeList **)malloc(sizeof(EdgeList *) * numVertices);
  for (int i = 0; i < numVertices; i++)
  {
    paths[i] = NULL;
  }
  
  for (int i = 0; i < numVertices; i++)
  {
    int begin = distTree[i].toVertex;
    int end = distTree[i].fromVertex;
    int new_dist = distTree[i].weight;
    // printf("%d\n", begin);
    if (end == startVertex)
    {
      continue;
    }
    EdgeList *tmp = paths[begin];
    while (tmp)
    {
      new_dist = new_dist - tmp->edge->weight;
      tmp = tmp->next;
    }
    Edge * edge = newEdge(end, begin, new_dist);
    paths[end] = newEdgeList(edge,cloneEdgeList(paths[begin]));
  }
  return paths;
}