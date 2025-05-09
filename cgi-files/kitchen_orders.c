#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        perror("Unable to open orders.txt");
        exit(1);
    }

    char line[256];
    printf("Content-Type: application/json\n\n");
    printf("[");

    int first = 1;
    while (fgets(line, sizeof(line), file)) {
        if (!first) {
            printf(",");
        }
        first = 0;

        char orderID[10], dishName[100], status[10];
        sscanf(line, "OrderID: %s, Dish: %s, Status: %s", orderID, dishName, status);

        printf("{\"id\": \"%s\", \"dish\": \"%s\", \"status\": \"%s\"}", orderID, dishName, status);
    }

    printf("]");
    fclose(file);
    return 0;
}
