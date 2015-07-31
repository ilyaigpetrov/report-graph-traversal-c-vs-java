#ifndef ProfilerH
#define ProfilerH

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Profiler Profiler;
struct Profiler {
    char **labels;
    long *elapsed;
    long *started;
    int length;
    int capacity;
};

#define PROFILER_INITIAL_SIZE 10
Profiler PROFILER_VALUE = { .length = 0, .capacity = PROFILER_INITIAL_SIZE };
Profiler *PROFILER = &PROFILER_VALUE;

void initializeProfiler_malloc() {
    PROFILER->labels   = malloc(sizeof(char*)*PROFILER_INITIAL_SIZE);
    PROFILER->elapsed  = malloc(sizeof(clock_t) *PROFILER_INITIAL_SIZE);
    PROFILER->started  = malloc(sizeof(clock_t) *PROFILER_INITIAL_SIZE);
}

void destroyProfiler_free() {
    free(PROFILER->labels);
    free(PROFILER->elapsed);
    free(PROFILER->started);
}

void guarantCapacity() {
    if (PROFILER->length == PROFILER->capacity) {
        int newCapacity = PROFILER->capacity * 2;
        char **newLabels = malloc(sizeof(char*) * newCapacity);
        clock_t *newElapsed = malloc(sizeof(clock_t) * newCapacity);
        clock_t *newStarted = malloc(sizeof(clock_t) * newCapacity);
        int i;
        for(i = 0; i < PROFILER->length; i++) {
            newLabels[i] = PROFILER->labels[i];
            newElapsed[i] = PROFILER->elapsed[i];
            newStarted[i] = PROFILER->started[i];
        }
        free(PROFILER->labels);
        free(PROFILER->elapsed);
        free(PROFILER->started);
    }
}

void PROFILER_startTimeProfile(char *name) {
    printf("PROFILER starts %s\n", name);
    guarantCapacity();
    clock_t start = clock() / (CLOCKS_PER_SEC / 1000);
    int i = PROFILER->length;
    PROFILER->started[i] = start;
    PROFILER->labels[i]  = name;
    PROFILER->length++;
}

void PROFILER_endTimeProfile(char *name) {

    clock_t now = clock() / (CLOCKS_PER_SEC / 1000);
    int n = PROFILER->length;
    
    char *prev = PROFILER->labels[n - 1];
    int cmp = strcmp(prev, name);
    if (cmp != 0) {
        failWith(prev," label is ended with ", name, " label in profiler.", NULL);
    }
    clock_t newElapsed = now - PROFILER->started[n - 1];
    PROFILER->elapsed[n - 1] = newElapsed;
    
    printf("PROFILER ends %s\n", name);
}

void PROFILER_writeProfiles(char *fname) {

    FILE *file = fopen(fname, "w");
    if (file == NULL)
        failWith("Error opening file for writing:", fname, NULL);

    int i;
    fprintf(file, "{\n");
    puts("Time spent:");
    for (i = 0; i < PROFILER->length; i++) {
        char *label = PROFILER->labels[i];
        clock_t elapsed = PROFILER->elapsed[i];
        fprintf(file, "\"%s\": %i,\n", label, elapsed);
        printf("\"%s\": %ims\n", label, elapsed);
    }
    fprintf(file, "\"\":\"\"\n");
    fprintf(file, "}\n");
    fclose(file);
}

#endif