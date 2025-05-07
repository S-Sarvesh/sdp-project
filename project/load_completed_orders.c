#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_header() {
    printf("Content-Type: application/json\n\n");
}

int main() {
    FILE *file;
    char line[256];
    char orders[1024] = "["; // Start of JSON array

    print_header();

    file = fopen("/path/to/completed_orders.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        printf("[]");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        char order_id[10], dish[100];
        // Assuming the file has order_id and dish separated by a comma
        sscanf(line, "%9s,%99[^\n]", order_id, dish);
        
        // Add to JSON array
        strcat(orders, "{\"id\":\"");
        strcat(orders, order_id);
        strcat(orders, "\",\"dish\":\"");
        strcat(orders, dish);
        strcat(orders, "\"},");
    }
    fclose(file);

    // Remove the last comma and close JSON array
    if (strlen(orders) > 1) {
        orders[strlen(orders) - 1] = ']';
    } else {
        strcat(orders, "]");
    }

    printf("%s", orders);
    return 0;
}
