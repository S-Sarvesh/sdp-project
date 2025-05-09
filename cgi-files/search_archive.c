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
} Order;

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

int extractOrderId(char *query) {
    char *idStr = strstr(query, "orderId=");
    if (!idStr) return -1;
    return atoi(idStr + 8);
}

int main() {
    printf("Content-Type: application/json\n\n");

    char *query = getenv("QUERY_STRING");
    if (!query) {
        printf("[]");
        return 0;
    }

    int searchId = extractOrderId(query);
    if (searchId == -1) {
        printf("[]");
        return 0;
    }

    FILE *file = fopen("archive.txt", "r");
    if (!file) {
        printf("[]");
        return 0;
    }

    printf("[");
    int first = 1;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        Order order;
        memset(&order, 0, sizeof(order));
        
        char *token = strtok(line, "|");
        if (!token) continue;
        order.id = atoi(token);
        
        if (searchId != -1 && order.id != searchId) continue;
        
        token = strtok(NULL, "|");
        if (token) strncpy(order.customer, token, sizeof(order.customer)-1);
        
        token = strtok(NULL, "|");
        if (token) strncpy(order.phone, token, sizeof(order.phone)-1);
        
        token = strtok(NULL, "|");
        if (token) strncpy(order.address, token, sizeof(order.address)-1);
        
        token = strtok(NULL, "|");
        if (token) strncpy(order.dish, token, sizeof(order.dish)-1);
        
        token = strtok(NULL, "|");
        if (token) {
            char *newline = strchr(token, '\n');
            if (newline) *newline = '\0';
            strncpy(order.status, token, sizeof(order.status)-1);
        }
        
        char escapedCustomer[256], escapedPhone[64], escapedAddress[512];
        char escapedDish[128], escapedStatus[64];
        
        escapeJsonString(escapedCustomer, order.customer, sizeof(escapedCustomer));
        escapeJsonString(escapedPhone, order.phone, sizeof(escapedPhone));
        escapeJsonString(escapedAddress, order.address, sizeof(escapedAddress));
        escapeJsonString(escapedDish, order.dish, sizeof(escapedDish));
        escapeJsonString(escapedStatus, order.status, sizeof(escapedStatus));

        if (!first) printf(",");
        printf("{\"id\":%d,\"customer\":\"%s\",\"phone\":\"%s\",\"address\":\"%s\",\"dish\":\"%s\",\"status\":\"%s\"}",
               order.id, escapedCustomer, escapedPhone, escapedAddress, escapedDish, escapedStatus);
        first = 0;
        
        if (searchId != -1) break; // We found our specific order
    }
    printf("]");
    fclose(file);
    return 0;
}