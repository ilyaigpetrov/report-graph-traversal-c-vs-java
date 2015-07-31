#ifndef GraphH
#define GraphH

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct CityInformation CityInformation;
struct CityInformation {
    char* city;
    double population;
    double elevation;
};

CityInformation* newCityInformation_malloc(char *city, double population, double elevation) {
    CityInformation *info = malloc ( sizeof(CityInformation) );
    size_t cityLen = strlen(city);
    info->city = (char*) malloc(cityLen + 1);
    strncpy(info->city, city, cityLen + 1); // Copy string, pointer may change.
    info->population = population;
    info->elevation = elevation;
    return info;
}

typedef struct ListNode ListNode;
struct ListNode {
    int number;
    ListNode *next;
};

ListNode* newListNode_malloc(int number) {
    ListNode *listNode = (ListNode*) malloc( sizeof(ListNode) );
    listNode->number = number;
    listNode->next = NULL;
    return listNode;
}

void destroyListNodeChain_free(ListNode* listNode) {
    while(listNode != NULL) {
        ListNode* next = listNode->next;
        free(listNode);
        listNode = next;
    }
}

typedef struct List List;
struct List {
    ListNode *first;  // To start traversal.
    ListNode *last;   // To add new nodes.
};

List* newList_malloc(int number) {
    List *list = (List*) malloc( sizeof(List) );
    list->first = list->last = newListNode_malloc(number);
    return list;
}

void destroyList_free(List* list) {
    destroyListNodeChain_free(list->first);
    free(list);
}

void ListAddListNode(List *list, int noden) {
    ListNode *last = list->last;
    last->next = list->last = newListNode_malloc(noden);
}

typedef struct Graph Graph;
struct Graph {
    CityInformation **nodes;
    List **edges; // Adjacency List
    int count;
    int source;
    int target;
    bool (*_ifTargetFunction)(Graph*, int);
};

bool GraphIfTarget(Graph *graph, int noden) {
    return graph->_ifTargetFunction(graph, noden);
}

ListNode* GraphGetNeighbours(Graph *graph, int from) {
    return graph->edges[from - 1]->first;
}

Graph* newGraph_malloc(int nodesCount) {
    Graph *graph = malloc(sizeof(Graph));
    graph->count = nodesCount;
    graph->nodes = calloc( nodesCount, sizeof(CityInformation) );
    graph->edges = calloc( nodesCount, sizeof(List*) );
    int i;
    for (i = 0; i < graph->count; i++) {
        graph->edges[i] = NULL; // 0s are not NULLs on some archs.
    }
    bool defaultIfTargetFunction(Graph *graph, int noden) {
        return graph->target == noden;
    }    
    graph->_ifTargetFunction = &defaultIfTargetFunction;
    return graph;
}

void freeArrayOfObjects_free(void **arr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        free(arr[i]);
    };
    free(arr);
}

void destroyGraph_free(Graph *graph) {
    int i;
    for(i = 0; i < graph->count; i++) {
        List* list = graph->edges[i];
        if (list != NULL) {
            destroyList_free(list);
        }
    }
    free(graph->edges);    
    freeArrayOfObjects_free((void**)graph->nodes, graph->count);
}

#endif