/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge *edge)
{
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList *head)
{
  while (head != NULL)
  {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex *vertex)
{
  if (vertex == NULL)
  {
    printf("NULL");
  }
  else
  {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph *graph)
{
  if (graph == NULL)
  {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++)
  {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge *newEdge(int fromVertex, int toVertex, int weight)
{
  Edge *res = (Edge *)malloc(sizeof(Edge));
  if (res == NULL){
    return NULL;
  }
  res->fromVertex = fromVertex;
  res->toVertex = toVertex;
  res->weight = weight;
  return res;
}

/* Returns a newly created EdgeList containing 'edge' and pointing to the next
 * EdgeList node 'next'.
 */
EdgeList *newEdgeList(Edge *edge, EdgeList *next)
{
  EdgeList *res = (EdgeList *)malloc(sizeof(EdgeList));
  if (res == NULL){
    return NULL;
  }
  res->edge = edge;
  res->next = next;
  return res;
}

/* Returns a newly created Vertex with ID 'id', value 'value', and adjacency
 * list 'adjList'.
 * Precondition: 'id' is valid for this vertex
 */
Vertex *newVertex(int id, void *value, EdgeList *adjList)
{
  Vertex *res =(Vertex *) malloc(sizeof(Vertex));
  if (res == NULL){
    return NULL;
  }
  res->adjList = adjList;
  res->id = id;
  res->value = value;
  return res;
}

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph* newGraph(int numVertices){
  Graph* res = (Graph*)malloc(1 * sizeof(Graph));
  if (res == NULL) {
    return NULL;
  }
  res->numVertices = numVertices;
  res->numEdges = 0;
  res->vertices = (Vertex**)malloc(numVertices * sizeof(Vertex*));
  if (res->vertices == NULL) {
    return NULL;
  }
  for (int i = 0; i < numVertices; i++) {
    res->vertices[i] = NULL;
  }
  return res;
}

/* Frees memory allocated for EdgeList starting at 'head'.
 */
void deleteEdgeList(EdgeList *head)
{
  if (head == NULL) 
  {
    return;
  }
  
  EdgeList* cur = head;
  EdgeList* tmp = NULL;
  while (cur!= NULL) {
    tmp = cur->next;
    free(cur->edge);
    free(cur);
    cur = tmp;
  }
}

/* Frees memory allocated for 'vertex' including its adjacency list.
 */
void deleteVertex(Vertex *vertex)
{
  deleteEdgeList(vertex->adjList);
  free(vertex);
}

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph *graph)
{
  for (int i = 0; i < graph->numVertices; i++)
  {
    deleteVertex(graph->vertices[i]);
  }
  free(graph->vertices);

  free(graph);
}

