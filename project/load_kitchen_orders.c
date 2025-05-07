// load_kitchen_orders.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(void) {
    FILE *fp = fopen("/path/to/your/pending_orders.txt", "r"); // Change the path properly
    if (!fp) {
        printf("Content-Type: application/json\n\n");
        printf("[]");
        return 0;
    }

    printf("Content-Type: application/json\n\n");

    printf("[");
    char line[MAX_LINE];
    int first = 1;

    while (fgets(line, sizeof(line), fp)) {
        char id[100], customer[100], phone[100], address[200], dish[100], status[50];
        
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", id, customer, phone, address, dish, status);

        // Only show orders with Pending status
        if (strcmp(status, "Pending") != 0) {
            continue;
        }

        if (!first) {
            printf(",");
        }
        first = 0;

        printf("{");
        printf("\"id\":\"%s\",", id);
        printf("\"customer\":\"%s\",", customer);
        printf("\"phone\":\"%s\",", phone);
        printf("\"address\":\"%s\",", address);
        printf("\"dish\":\"%s\",", dish);
        printf("\"status\":\"%s\"", status);
        printf("}");
    }

    printf("]");
    fclose(fp);
    return 0;
}
