#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char dish[100];
    
    // Read POST data
    char *data = getenv("QUERY_STRING");
    if (data == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("Error: No data received");
        return 1;
    }
    
    // Parse the dish name
    sscanf(data, "dish=%s", dish);
    
    // Open the file in append mode
    FILE *file = fopen("orders.txt", "a");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("Error: Could not open file");
        return 1;
    }
    
    // Write the order to the file
    fprintf(file, "OrderID: %d, Dish: %s, Status: Pending\n", rand(), dish);
    
    fclose(file);
    
    // Return a success message to the client
    printf("Content-Type: text/html\n\n");
    printf("Order added successfully: %s", dish);
    return 0;
}
