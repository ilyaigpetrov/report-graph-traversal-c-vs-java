#ifndef ToolH
#define ToolH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Graph.h"

const int MAXIMUM_CITY_NAME_LENGTH = 30;
int MAXIMUM_EXP_LENGTH = 100;

char* concatValist_malloc(va_list argp) {
    char *message = (char*) malloc(1);
    *message = 0;
    int messageLen = strlen(message);
    char *string = va_arg(argp, char*);
    while( string != NULL ) {   
        int stringLen = strlen(string);
        messageLen += stringLen;
        message = (char*) realloc(message, messageLen + 1); // Length + /0
        strncat(message, string, stringLen + 1);
        string = va_arg(argp, char*);
    }
    return message;
}

char* concat_malloc(char *first, ...) {
    size_t messageLen = strlen(first);
    char *message = (char*) malloc(messageLen + 1);
    strncpy(message, first, messageLen + 1);

    va_list argp;
    va_start(argp, first);
    char *tail = concatValist_malloc(argp);
        int tailLen = strlen(tail);
        messageLen += tailLen;
        message = (char*) realloc(message, messageLen + 1); // Length + /0
        strncat(message, tail, tailLen + 1);
    free(tail);
    va_end(argp);
    return message;
}


void failWith(char *first, ...) {
    va_list argp;
    va_start(argp, first);
    char *tail = concatValist_malloc(argp);
    va_end(argp);
    char *message = concat_malloc(first, tail, NULL);
    free(tail);
    printf("Error: %s", message);
    free(message);
    exit(EXIT_FAILURE);
}

typedef struct FileStream FileStream;
struct FileStream {
    FILE * fp;
};

FileStream* newFileStream_malloc(char* fname) {
    FILE *fp;

    fp = fopen(fname, "r");
    if (fp == NULL)
        failWith("Couldn't open file: ", fname, NULL);

    FileStream *fs = malloc ( sizeof(FileStream) );
    fs->fp = fp;
    return fs;
}

void destroyFileStream_free(FileStream *fs) {
    if (fs->fp != NULL)
        fclose(fs->fp);
    free(fs);
}

bool isEmpty(char *s) {
    while (*s) {
        if (!isspace(*s)) return false; 
        s++;
    }
    return true;
}

/*
  This getline is crossplatfrom but very slow.
*/
int crossplatformGetline(char **line, int *lineSize, FILE *fp) {
    int i = 0;
    while(true) {
        while(i < *lineSize) {
            char c = fgetc(fp);
            if (c != '\n' && c != '\r' && c != EOF) {
                (*line)[i++] = c;
                continue;
            } else {
                (*line)[i] = 0;
                i++;
                if (c == '\r') {
                    char nextc = fgetc(fp);
                    if (nextc != '\n') {
                        ungetc(nextc, fp);
                    } else c = nextc;
                    i++;
                }
                if (c == EOF && i == 1)
                    return -1;
                return i;
            }
        }
        if (*lineSize == 0)
            *lineSize = 20000;
        *lineSize = (*lineSize) * 2;
        *line = (char*) realloc(*line, *lineSize + 1); // Length + /0
    }
}

char* FileStreamReadLine_malloc(FileStream* fs) {
    char *line = NULL;
    size_t len = 0;
    ssize_t charsRead;
    
    do {
        charsRead = getline(&line, &len, fs->fp);
    } while (charsRead != -1 && isEmpty(line));
    if (charsRead == -1) {
        return NULL;
    }
    int lineLen = strlen(line);
    return line;
}

char* StringTrimTail(char* string) {
    char *end = string + strlen(string) - 1;
    while(end > string && isspace(*end)) end--;
    // Write new null terminator.
    *(end+1) = 0;
    return string;
}

Graph* readGraph_malloc(int nodesCount, bool ifIgnoreTarget) {

    Graph *graph = newGraph_malloc(nodesCount);

    FileStream *fs = newFileStream_malloc("Node_Information_of_Graph_G.txt");
    
    puts("Reading node information...");
    char *line = FileStreamReadLine_malloc(fs);
        if (line != NULL)
            free(line); // Skip the header if any.
        
        while ((line = FileStreamReadLine_malloc(fs)) != NULL) {
            int number = -1;
            char* cityName = (char*)malloc( sizeof(char) * MAXIMUM_CITY_NAME_LENGTH );
            double population, elevation;
            int numberMatched = sscanf (line, "%i %[a-zA-Z '] %lf %lf", &number, cityName, &population, &elevation);
            if (numberMatched != 4) {
                failWith("Can't parse this as city info: ", line, "!", NULL);
            }
            graph->nodes[number - 1] = newCityInformation_malloc(StringTrimTail(cityName), population, elevation);
            free(cityName);
            free(line);
        }
    destroyFileStream_free(fs);
    puts("finished.");

    puts("Reading adjacency matrix...");
    fs = newFileStream_malloc("AdjacencyMatrix_of_Graph_G.txt");
        int linei = 0;
        int len = 0;
        while ((line = FileStreamReadLine_malloc(fs)) != NULL) {
            printf("Line %i of adj\n", linei);
            int toi;
            int i = 0;
            for (toi = 0; toi < graph->count; toi++) {
                char ch = line[i++];
                while (ch == ' ' || ch == '\t') {
                    ch = line[i++];
                }
                bool ifAdj = ch == '1';
                if (ifAdj) {
                    List* list = graph->edges[linei];
                    if (list == NULL) {
                        graph->edges[linei] = newList_malloc(toi + 1);
                    } else {
                        ListAddListNode(list, toi + 1);
                    }
                } else if (ch != '0') {
                    char buffer[200];
                    sprintf(buffer, "Character %c on line %i can't be parsed.", ch, linei);
                    failWith(buffer, NULL);
                }
            }

            linei++;
            free(line);
        }    
    destroyFileStream_free(fs);
    puts("finished.");
    
    fs = newFileStream_malloc("SourceNode_StoppingCondition.txt");
        int source;
        int target = -1;
        graph->target = target;
        line = FileStreamReadLine_malloc(fs);
        if (line != NULL)
            free(line); // Skip the header if any.
        line = FileStreamReadLine_malloc(fs);
        char* exp = (char*)malloc( sizeof(char) * MAXIMUM_EXP_LENGTH );
            int numberMatched = sscanf (line, "%d%*[ ]%[\"]", &source, &target); // TODO: specify target as digit.
            if (numberMatched == 2) {
                if (ifIgnoreTarget == false) {
                    numberMatched = sscanf (line, "%i %i", &source, &target);
                    if (numberMatched != 2) {
                        numberMatched = sscanf (line, "%i \"%[^\"]\"", &source, exp);
                        if (numberMatched != 2)
                            failWith("Second token is not an expression or number:", line, "!", NULL);
                        #ifndef EXPRESSION
                            failWith("EXPRESSION must be also defined in Expression.h input file. Define it and recompile.", NULL);
                        #else
                        bool checkExpression(Graph *graph, int noden) {
                            CityInformation *info = graph->nodes[noden - 1];
                            char *city = info->city;
                            double pop = info->population;
                            double elevation = info->elevation;
                            bool CityIs(char *name) {
                                return strcmp(city, name) == 0;
                            }
                            if (EXPRESSION) {
                                return true;
                            } else {
                                return false;
                            }
                        }
                        graph->_ifTargetFunction = &checkExpression;
                        #endif
                    } // else TODO: read int as target.
                }
                target = -1;
            } else if (numberMatched != 1) {
                failWith("Can't parse source-target specification:", line, "!", NULL);
            }
        free(exp);
        free(line);
        graph->source = source;
    destroyFileStream_free(fs);
    
    return graph;
}

#endif