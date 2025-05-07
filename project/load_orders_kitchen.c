#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDER_LENGTH 256

typedef struct Order {
    int id;
    char dish[MAX_ORDER_LENGTH];
    char status[MAX_ORDER_LENGTH];
    struct Order* next;
} Order;

Order* load_orders(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open orders.txt");
        return NULL;
    }

    Order* head = NULL;
    Order* tail = NULL;

    char line[MAX_ORDER_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        Order* new_order = (Order*)malloc(sizeof(Order));
        if (!new_order) {
            perror("Unable to allocate memory for new order");
            fclose(file);
            return NULL;
        }

        // Parse the line into the order structure
        sscanf(line, "%d|%[^|]|%s", &new_order->id, new_order->dish, new_order->status);
        new_order->next = NULL;

        if (tail) {
            tail->next = new_order;
        } else {
            head = new_order;
        }
        tail = new_order;
    }

    fclose(file);
    return head;
}

void print_order_json(Order* head) {
    printf("Content-Type: application/json\n\n");
    printf("[");
    Order* temp = head;
    while (temp) {
        printf("{\"id\": %d, \"dish\": \"%s\", \"status\": \"%s\"}", temp->id, temp->dish, temp->status);
        if (temp->next) {
            printf(",");
        }
        temp = temp->next;
    }
    printf("]\n");
}

int main() {
    // Load orders from the file
    Order* orders = load_orders("orders.txt");

    // Output orders as JSON
    print_order_json(orders);

    // Free memory
    Order* temp;
    while (orders) {
        temp = orders;
        orders = orders->next;
        free(temp);
    }

    return 0;
}
