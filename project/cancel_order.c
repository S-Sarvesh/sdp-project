#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to simulate database of orders (You may replace this with file or database operations)
typedef struct Order {
    int id;
    char customer[100];
    char phone[15];
    char address[255];
    char dish[50];
    char status[50];
    struct Order *next;
} Order;

Order *head = NULL; // Head of the active orders list

// Function to find and cancel an order
int cancel_order(int orderId) {
    Order *current = head;
    Order *prev = NULL;

    while (current != NULL) {
        if (current->id == orderId) {
            strcpy(current->status, "Cancelled");
            // Move to archived orders (this is just a simulation, adjust as needed)
            printf("Content-type: application/json\n\n");
            printf("{\"success\": true, \"message\": \"Order cancelled.\"}\n");
            return 1; // Order found and cancelled
        }
        prev = current;
        current = current->next;
    }

    return 0; // Order not found
}

int main() {
    // Read the orderId from the GET request
    char *query_string = getenv("QUERY_STRING");
    if (query_string == NULL) {
        printf("Content-type: text/html\n\n");
        printf("Invalid request.\n");
        return 1;
    }

    // Parse the query string to get orderId
    char orderIdStr[10];
    sscanf(query_string, "orderId=%s", orderIdStr);
    int orderId = atoi(orderIdStr);

    // Try to cancel the order
    if (cancel_order(orderId)) {
        return 0;
    } else {
        printf("Content-type: application/json\n\n");
        printf("{\"success\": false, \"message\": \"Order not found.\"}\n");
    }

    return 0;
}
