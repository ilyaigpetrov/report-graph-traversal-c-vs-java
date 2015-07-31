#include "TraverseAlgorithm.h"

typedef struct Queue Queue;
struct Queue {
    List* list;
    int size;
};

Queue* newQueue_malloc() {
    Queue* queue = malloc(sizeof(Queue));
    // Phony -1 is always the last element so we can add to queue without checking if queue is empty.
    queue->list = newList_malloc(-1); // TODO:
    queue->size = 0;
    return queue;
}

void QueueAdd(Queue *queue, int noden) {
    ListNode *last = queue->list->last;
    last->number = noden;
    ListAddListNode(queue->list, -1);
    queue->size++;
}

int QueueRemove(Queue *queue) {
    ListNode *first = queue->list->first;
    int noden = first->number;
    if (noden == -1) {
        // Queue is empty.
        return noden;
    }
    queue->list->first = first->next;
    free(first);
    queue->size--;
    return noden;
}


void destroyQueue_free(Queue *queue) {
    destroyList_free(queue->list);
    free(queue);
}

typedef struct BFS BFS;
struct BFS {
    Graph *graph;
    bool *visited;
    Queue *visitQueue;
    FILE *_file;
    int visitedCount;
};

BFS* newBFS_malloc(Graph *graph) {
    BFS *bfs = malloc(sizeof(BFS));
    bfs->graph = graph;
    bfs->visited = calloc( graph->count, sizeof(bool) );
    bfs->visitQueue = newQueue_malloc();
    bfs->visitedCount = 0;
    return bfs;
}

void destroyBFS_free(BFS *bfs) {
    free(bfs->visited);
    destroyQueue_free(bfs->visitQueue);
    free(bfs);
}

bool BFSVisitAndCheck(BFS *bfs, int noden) {
    CityInformation *info = bfs->graph->nodes[noden - 1];
    char *template = "(City: %s, Pop: %i, Elevation: %.1f)\n";
    fprintf(bfs->_file, template, info->city, (int)info->population, info->elevation);
    printf(template, info->city, (int)info->population, info->elevation);
    bfs->visited[noden - 1] = true;
    bfs->visitedCount++;
    return GraphIfTarget(bfs->graph, noden);
}

/*

  Pseudo code

    bfs(from vertex v)
      visit v
      list L = v
      while L nonempty
          choose some vertex v from the front of the list
          for each not visited neighbor w of v
            visit w
            add w it to end of L
    
*/
void _BFSRun(BFS *bfs, int start) {
    QueueAdd(bfs->visitQueue, start);
    if (BFSVisitAndCheck(bfs, start))
        return;
    while(bfs->visitQueue->size != 0) {
        int noden = QueueRemove(bfs->visitQueue);
        ListNode *nbr = GraphGetNeighbours(bfs->graph, noden);
        for( ; nbr != NULL; nbr = nbr->next ) {
            if (bfs->visited[nbr->number - 1] == false) {
                if (BFSVisitAndCheck(bfs, nbr->number))
                    return;
                QueueAdd(bfs->visitQueue, nbr->number);
            }
        }
    }
}

void BFSRun(BFS *bfs) {
    _BFSRun(bfs, bfs->graph->source);
}

int RunBFSAlgorithm(Graph *graph, FILE *file) {
    BFS* bfs = newBFS_malloc(graph);
    bfs->_file = file;
    BFSRun(bfs);
    int result = bfs->visitedCount;
    destroyBFS_free(bfs);
    return result;
}

int
main(int argc, char **argv) {
    runTraverseAlgorithm(argc, argv, "BFS", &RunBFSAlgorithm);
}

