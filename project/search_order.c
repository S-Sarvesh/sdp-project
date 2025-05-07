#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Order structure
typedef struct Order {
    int id;
    char customer[100];
    char phone[20];
    char address[200];
    char dish[50];
    struct Order *left, *right;
} Order;

// Function to escape JSON special characters
void escapeJsonString(char *dest, const char *src, size_t destSize) {
    size_t i = 0;
    for (; *src && i < destSize - 2; src++) {
        if (*src == '"' || *src == '\\') {
            if (i < destSize - 3) {
                dest[i++] = '\\';
                dest[i++] = *src;
            }
        } else if (*src == '\n') {
            if (i < destSize - 3) {
                dest[i++] = '\\';
                dest[i++] = 'n';
            }
        } else {
            dest[i++] = *src;
        }
    }
    dest[i] = '\0';
}

// Insert order into the binary search tree
Order* insert(Order* root, Order* newOrder) {
    if (!root) return newOrder;
    if (newOrder->id < root->id)
        root->left = insert(root->left, newOrder);
    else
        root->right = insert(root->right, newOrder);
    return root;
}

// Search for an order by ID
Order* search(Order* root, int id) {
    if (!root || root->id == id) return root;
    if (id < root->id)
        return search(root->left, id);
    else
        return search(root->right, id);
}

// Parse a line into an Order structure
Order* createOrderFromLine(char* line) {
    Order* order = (Order*)malloc(sizeof(Order));
    if (!order) return NULL;
    sscanf(line, "%d,%99[^,],%19[^,],%199[^,],%49[^\n]",
           &order->id, order->customer, order->phone, order->address, order->dish);
    order->left = order->right = NULL;
    return order;
}

// Extract the orderId from the query string
int extractOrderId(char *query) {
    char *idStr = strstr(query, "orderId=");
    if (!idStr) return -1;
    return atoi(idStr + 8);
}

int main() {
    printf("Content-Type: application/json\n\n");

    char *query = getenv("QUERY_STRING");
    if (!query) {
        printf("{\"found\": false, \"error\": \"No query string\"}");
        return 0;
    }

    int searchId = extractOrderId(query);
    if (searchId == -1) {
        printf("{\"found\": false, \"error\": \"Invalid or missing orderId\"}");
        return 0;
    }

    FILE *file = fopen("orders.txt", "r");
    if (!file) {
        printf("{\"found\": false, \"error\": \"Cannot open orders.txt\"}");
        return 0;
    }

    Order *root = NULL;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        Order *order = createOrderFromLine(line);
        if (order)
            root = insert(root, order);
    }
    fclose(file);

    Order *result = search(root, searchId);
    if (result) {
        // Buffers for escaping JSON data
        char escapedCustomer[256], escapedDish[128], escapedPhone[64], escapedAddress[512];

        escapeJsonString(escapedCustomer, result->customer, sizeof(escapedCustomer));
        escapeJsonString(escapedPhone, result->phone, sizeof(escapedPhone));
        escapeJsonString(escapedAddress, result->address, sizeof(escapedAddress));
        escapeJsonString(escapedDish, result->dish, sizeof(escapedDish));

        // Output the found order in JSON format
        printf("{\"found\": true, \"order\": {"
               "\"id\": %d, "
               "\"customer\": \"%s\", "
               "\"phone\": \"%s\", "
               "\"address\": \"%s\", "
               "\"dish\": \"%s\"}}",
               result->id, escapedCustomer, escapedPhone, escapedAddress, escapedDish);
    } else {
        printf("{\"found\": false}");
    }

    return 0;
}
