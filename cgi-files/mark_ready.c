#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assuming you have a function to update the order status in your database
void mark_order_ready(int order_id) {
    // Update the order status to "Ready" in the database
    // This will depend on how your database is set up (e.g., using SQL queries)
    // Example: update order status in the database
    char query[256];
    sprintf(query, "UPDATE orders SET status = 'Ready to Collect' WHERE id = %d", order_id);
    // Execute your database query here (e.g., using MySQL or SQLite)
}

int main(void) {
    char *id_str = getenv("QUERY_STRING");  // Get the order ID from the query string
    if (id_str != NULL) {
        int order_id = atoi(id_str);  // Convert the ID to an integer
        mark_order_ready(order_id);  // Update the order status
        printf("Content-Type: application/json\n\n");
        printf("{\"message\": \"Order marked as ready!\"}");
    } else {
        printf("Content-Type: application/json\n\n");
        printf("{\"message\": \"Error: No order ID provided!\"}");
    }
    return 0;
}
