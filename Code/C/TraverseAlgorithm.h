#ifndef TraverseAlgorithmH
#define TraverseAlgorithmH

#include "Tool.h"
#include "Profiler.h"

runTraverseAlgorithm(int argc, char **argv, char *algoName, int (*runAlgorithm)(Graph *graph, FILE *file)) {
    argv++;
    argc--;
    if ( argc != 1 && argc != 2 )
        failWith("Missing required arguments: NumberOfNodes [all]", NULL);
    int nodesCount;
    bool ifIgnoreTarget = false;
    if (sscanf (argv[0], "%i", &nodesCount) != 1) {
        failWith("First argument is not an integer: ", argv[0], NULL);
    }
    if (argc == 2) {
        char all[40];
        if (sscanf (argv[1], "%s", &all) != 1 || strcmp(all, "all") != 0)
            failWith("Something else instead of 'all' argument supplied.", NULL);
        ifIgnoreTarget = true;
    }

    initializeProfiler_malloc();
        PROFILER_startTimeProfile("INPUT");
        Graph *graph = readGraph_malloc(nodesCount, ifIgnoreTarget);
        char *algoLabel = concat_malloc("ALGO:", algoName, NULL);            
            PROFILER_endTimeProfile("INPUT");
            PROFILER_startTimeProfile(algoLabel);
            char *fname = concat_malloc("Result_of_", algoName,"_from_Source_to_Target_N_X.txt", NULL);
                FILE *file = fopen(fname, "w");
                    if (file == NULL)
                        failWith("Error opening file for writing:", fname, NULL);
                    int visitedCount = runAlgorithm(graph, file);
                fclose(file);
                char res[20];
                sprintf(res, "%i", visitedCount);
                char *newName = concat_malloc("Result_of_", algoName,"_from_Source_to_Target_N_", res, ".txt", NULL);
                rename(fname, newName);
                free(newName);
            free(fname);
            PROFILER_endTimeProfile  (algoLabel);
            printf("Nodes visited: %i\n", visitedCount);
            char *pfile = concat_malloc("Profile_of_", algoName,"_from_Source_to_Target_N_", res, ".txt", NULL);
            PROFILER_writeProfiles(pfile);
            free(pfile);
        free(algoLabel);
    destroyProfiler_free();
}

#endif