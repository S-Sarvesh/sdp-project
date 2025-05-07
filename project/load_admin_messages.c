#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "admin_messages.txt"
#define MAX_MSG 500

int main() {
    printf("Content-Type: application/json\r\n\r\n");
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("[]");
        return 0;
    }

    char line[MAX_MSG];
    int first = 1;

    printf("[");
    while (fgets(line, sizeof(line), fp)) {
        // remove newline
        line[strcspn(line, "\r\n")] = 0;
        if (!first) printf(",");
        printf("\"%s\"", line);
        first = 0;
    }
    printf("]");
    fclose(fp);
    return 0;
}
