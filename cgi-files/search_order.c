#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Order {
    int id;
    char customer[100];
    char phone[20];
    char address[200];
    char dish[50];
    char status[20];
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

// Extract the orderId from the query string
int extractOrderId(char *query) {
    char *idStr = strstr(query, "orderId=");
    if (!idStr) return -1;
    return atoi(idStr + 8);
}

Order* createOrderFromLine(char* line) {
    Order* order = (Order*)malloc(sizeof(Order));
    if (!order) return NULL;
    
    // Initialize all fields
    memset(order, 0, sizeof(Order));
    
    // Tokenize by pipe character
    char *token = strtok(line, "|");
    if (!token) {
        free(order);
        return NULL;
    }
    order->id = atoi(token);
    
    token = strtok(NULL, "|");
    if (token) strncpy(order->customer, token, sizeof(order->customer)-1);
    
    token = strtok(NULL, "|");
    if (token) strncpy(order->phone, token, sizeof(order->phone)-1);
    
    token = strtok(NULL, "|");
    if (token) strncpy(order->address, token, sizeof(order->address)-1);
    
    token = strtok(NULL, "|");
    if (token) strncpy(order->dish, token, sizeof(order->dish)-1);
    
    token = strtok(NULL, "|");
    if (token) {
        // Remove any trailing newline characters from status
        char *newline = strchr(token, '\n');
        if (newline) *newline = '\0';
        strncpy(order->status, token, sizeof(order->status)-1);
    }
    
    order->left = order->right = NULL;
    return order;
}

int main() {
    printf("Content-Type: application/json\n\n");

    char *query = getenv("QUERY_STRING");
    if (!query) {
        printf("{\"found\":false,\"error\":\"No query string\"}");
        return 0;
    }

    int searchId = extractOrderId(query);
    if (searchId == -1) {
        printf("{\"found\":false,\"error\":\"Invalid order ID\"}");
        return 0;
    }

    FILE *file = fopen("orders.txt", "r");
    if (!file) {
        printf("{\"found\":false,\"error\":\"Cannot open orders file\"}");
        return 0;
    }

    Order *root = NULL;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters
        line[strcspn(line, "\r\n")] = '\0';
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        Order *order = createOrderFromLine(line);
        if (order) {
            root = insert(root, order);
        }
    }
    fclose(file);

    Order *result = search(root, searchId);
    if (result) {
        char escapedCustomer[256], escapedPhone[64], escapedAddress[512];
        char escapedDish[128], escapedStatus[64];
        
        escapeJsonString(escapedCustomer, result->customer, sizeof(escapedCustomer));
        escapeJsonString(escapedPhone, result->phone, sizeof(escapedPhone));
        escapeJsonString(escapedAddress, result->address, sizeof(escapedAddress));
        escapeJsonString(escapedDish, result->dish, sizeof(escapedDish));
        escapeJsonString(escapedStatus, result->status, sizeof(escapedStatus));

        printf("{\"found\":true,\"order\":{"
               "\"id\":%d,"
               "\"customer\":\"%s\","
               "\"phone\":\"%s\","
               "\"address\":\"%s\","
               "\"dish\":\"%s\","
               "\"status\":\"%s\""
               "}}",
               result->id, escapedCustomer, escapedPhone, escapedAddress,
               escapedDish, escapedStatus);
    } else {
        printf("{\"found\":false}");
    }

    return 0;
}