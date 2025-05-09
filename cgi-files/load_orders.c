#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp = fopen("orders.txt", "r");
    if (fp == NULL) {
        printf("Content-Type: application/json\n\n");
        printf("[]"); // Empty array if file not found
        return 0;
    }

    char line[1024];
    printf("Content-Type: application/json\n\n");
    printf("[\n");

    int first = 1;
    while (fgets(line, sizeof(line), fp)) {
        char id[100], customer[100], phone[100], address[200], dish[100], status[100];
        // Assuming orders.txt format is now: id|customer|phone|address|dish|status
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s", id, customer, phone, address, dish, status) == 6) {
            if (!first) {
                printf(",\n");
            }
            printf("{\"id\": \"%s\", \"customer\": \"%s\", \"phone\": \"%s\", \"address\": \"%s\", \"dish\": \"%s\", \"status\": \"%s\"}",
                   id, customer, phone, address, dish, status);
            first = 0;
        }
    }

    printf("\n]");n
    fclose(fp);
    return 0;
}
