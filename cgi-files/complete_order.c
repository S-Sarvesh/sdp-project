#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100

typedef struct {
    int id;
    char customer[100];
    char phone[20];
    char address[100];
    char dish[100];
    char status[20];
} Order;

Order orders[MAX_ORDERS];
int orderCount = 0;

void loadOrders() {
    FILE *file = fopen("/path/to/your/orders.txt", "r"); // Update path
    if (!file) return;
    
    orderCount = 0;
    while (fscanf(file, "%d|%99[^|]|%19[^|]|%99[^|]|%99[^|]|%19[^\n]\n",
                  &orders[orderCount].id,
                  orders[orderCount].customer,
                  orders[orderCount].phone,
                  orders[orderCount].address,
                  orders[orderCount].dish,
                  orders[orderCount].status) == 6) {
        orderCount++;
    }
    fclose(file);
}

void saveOrders() {
    FILE *file = fopen("/path/to/your/orders.txt", "w"); // Update path
    if (!file) return;

    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%d|%s|%s|%s|%s|%s\n",
                orders[i].id,
                orders[i].customer,
                orders[i].phone,
                orders[i].address,
                orders[i].dish,
                orders[i].status);
    }
    fclose(file);
}

void moveToArchive(Order completedOrder) {
    FILE *file = fopen("/path/to/your/archive.txt", "a"); // New file for archive
    if (!file) return;

    fprintf(file, "%d|%s|%s|%s|%s|Completed\n",
            completedOrder.id,
            completedOrder.customer,
            completedOrder.phone,
            completedOrder.address,
            completedOrder.dish);
    fclose(file);
}

int main() {
    printf("Content-Type: text/plain\n\n");

    char data[1024];
    fgets(data, sizeof(data), stdin);

    char *order_id_str = strstr(data, "order_id=");
    if (!order_id_str) {
        printf("No Order ID provided.");
        return 0;
    }

    int orderId = atoi(order_id_str + 9);

    loadOrders();

    int found = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].id == orderId) {
            // Move to archive
            moveToArchive(orders[i]);
            // Remove from current orders
            for (int j = i; j < orderCount - 1; j++) {
                orders[j] = orders[j+1];
            }
            orderCount--;
            found = 1;
            break;
        }
    }

    saveOrders();

    if (found) {
        printf("Order marked as completed successfully.");
    } else {
        printf("Order not found.");
    }

    return 0;
}
