#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.c"
#include "graph_algos.c"
#include "minheap.c"

// Helper function to add an undirected edge to the graph
void addUndirectedEdge(Graph *graph, int from, int to, int weight)
{
    Edge *edge1 = newEdge(from, to, weight);
    EdgeList *edgeList1 = newEdgeList(edge1, graph->vertices[from]->adjList);
    graph->vertices[from]->adjList = edgeList1;

    Edge *edge2 = newEdge(to, from, weight);
    EdgeList *edgeList2 = newEdgeList(edge2, graph->vertices[to]->adjList);
    graph->vertices[to]->adjList = edgeList2;

    graph->numEdges += 2;
}

// Test function to verify the correctness of the getMSTprim function
void testGetMSTprim()
{
    // Create a graph with 4 vertices
    Graph *graph = newGraph(4);

    // Initialize vertices
    for (int i = 0; i < graph->numVertices; i++)
    {
        graph->vertices[i] = newVertex(i, NULL, NULL);
    }

    // Add undirected edges
    addUndirectedEdge(graph, 0, 1, 10);
    addUndirectedEdge(graph, 0, 2, 6);
    addUndirectedEdge(graph, 0, 3, 5);
    addUndirectedEdge(graph, 1, 3, 15);
    addUndirectedEdge(graph, 2, 3, 4);

    // Get the MST starting from vertex 0
    Edge *mst = getMSTprim(graph, 0);

    // The MST should contain 3 edges for a graph with 4 vertices
    assert(mst != NULL);

    // Sum the weights of the MST and compare with expected MST weight
    int totalWeight = 0;
    for (int i = 0; i < graph->numVertices - 1; i++)
    {
        totalWeight += mst[i].weight;
        printEdge(&mst[i]);
    }

    // The expected MST weight is 19
    assert(totalWeight == 19);

    // Clean up the graph and MST
    deleteGraph(graph);
    free(mst);
}

int main()
{
    Graph *graph = newGraph(4);

    // Initialize vertices
    for (int i = 0; i < graph->numVertices; i++)
    {
        graph->vertices[i] = newVertex(i, NULL, NULL);
    }

    // Add undirected edges
    addUndirectedEdge(graph, 0, 1, 7);
    addUndirectedEdge(graph, 0, 2, 10);
    addUndirectedEdge(graph, 0, 3, 5);
    addUndirectedEdge(graph, 1, 3, 15);
    addUndirectedEdge(graph, 2, 3, 4);
    Edge *res = getDistanceTreeDijkstra(graph, 1);

    EdgeList **rres= getShortestPaths(res, 4, 1);

    printEdgeList(rres[0]);
    printf("\n");
    printEdgeList(rres[1]);
    printf("\n");
    printEdgeList(rres[2]);
    printf("\n");
    printEdgeList(rres[3]);
    return 0;
}
