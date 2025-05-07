#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Get the length of the incoming POST data
    char *data = getenv("CONTENT_LENGTH");
    int len = data ? atoi(data) : 0;

    // Set the content type to JSON
    printf("Content-Type: application/json\n\n");

    // If no data is received, return an error
    if (len <= 0) {
        printf("{\"status\":\"No data received\"}");
        return 0;
    }

    // Read the incoming POST data
    char postdata[1024];
    fgets(postdata, sizeof(postdata), stdin);

    // Parse the form data (id, customer, phone, address, and dish)
    char id[100], customer[100], phone[100], address[200], dish[100];
    sscanf(postdata, "id=%[^&]&customer=%[^&]&phone=%[^&]&address=%[^&]&dish=%[^&]",
           id, customer, phone, address, dish);

    // Optional: decode any encoded characters like '+' for spaces or '%20' for spaces (not implemented here)

    // Open the orders file to append the new order
    FILE *fp_orders = fopen("orders.txt", "a");
    if (fp_orders == NULL) {
        printf("{\"status\":\"Failed to open orders.txt\"}");
        return 0;
    }

    // Open the pending_orders file to append the new order
    FILE *fp_pending_orders = fopen("pending_orders.txt", "a");
    if (fp_pending_orders == NULL) {
        printf("{\"status\":\"Failed to open pending_orders.txt\"}");
        fclose(fp_orders);
        return 0;
    }

    // Write the order data to both files (id, customer, phone, address, dish, and default status)
    fprintf(fp_orders, "%s|%s|%s|%s|%s|Pending\n", id, customer, phone, address, dish);
    fprintf(fp_pending_orders, "%s|%s|%s|%s|%s|Pending\n", id, customer, phone, address, dish);

    // Close both files
    fclose(fp_orders);
    fclose(fp_pending_orders);

    // Return success message
    printf("{\"status\":\"Order Added Successfully\"}");
    return 0;
}
