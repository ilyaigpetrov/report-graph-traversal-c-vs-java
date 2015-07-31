#include <stdio.h>
#include <stdlib.h>


int
main(void) {

    puts("start");
    FILE *fp = fopen("input.txt", "r");
    int READ_BUFFER_SIZE = 1005;
    char *buffer = malloc( sizeof(char)*READ_BUFFER_SIZE );
    char *res = fgets(buffer, READ_BUFFER_SIZE, fp);
    printf("!%s!", res);
    res = fgets(buffer, READ_BUFFER_SIZE, fp);
    printf("!%s!", res);
    return 0;
}
