#include "TraverseAlgorithm.h"

typedef struct DFS DFS;
struct DFS {
    Graph *graph;
    bool *visited;
    FILE *_file;
    int visitedCount;
};

DFS* newDFS_malloc(Graph *graph) {
    DFS *dfs = malloc(sizeof(DFS));
    dfs->graph = graph;
    dfs->visited = calloc( graph->count, sizeof(bool) );
    dfs->visitedCount = 0;
    return dfs;
}

void destroyDFS_free(DFS *dfs) {
    free(dfs->visited);
    free(dfs);
}

void DFSVisit(DFS *dfs, int noden) {
    CityInformation *info = dfs->graph->nodes[noden - 1];
    char *template = "(City: %s, Pop: %i, Elevation: %.1f)\n";
    fprintf(dfs->_file, template, info->city, (int)info->population, info->elevation);
    printf(template, info->city, (int)info->population, info->elevation);
    dfs->visited[noden - 1] = true;
    dfs->visitedCount++;
}

/*

  Pseudo code

    dfs(from vertex v)
      visit(v)
      for each not visited neighbor w of v
        dfs(w)
        add edge vw to tree T

*/

bool _DFSRun(DFS *dfs, int from) {
    DFSVisit(dfs, from);
    if (GraphIfTarget(dfs->graph, from)) {
        return true;
    }
    ListNode *nbr = GraphGetNeighbours(dfs->graph, from);
    for( ; nbr != NULL; nbr = nbr->next) {
        if (dfs->visited[nbr->number - 1] == false) {
            bool ifFound = _DFSRun(dfs, nbr->number);
            if (ifFound)
                return true;
        }
    }
    return false;
}

void DFSRun(DFS *dfs) {
    _DFSRun(dfs, dfs->graph->source);
}

int RunDFSAlgorithm(Graph *graph, FILE *file) {
    DFS* dfs = newDFS_malloc(graph);
    dfs->_file = file;
    DFSRun(dfs);
    int result = dfs->visitedCount;
    destroyDFS_free(dfs);
    return result;
}

int
main(int argc, char **argv) {
    runTraverseAlgorithm(argc, argv, "DFS", &RunDFSAlgorithm);
}

