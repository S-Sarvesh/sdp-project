#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 255

// Define the Order structure
typedef struct Order {
    int id;
    char customer[MAX_LENGTH];
    char phone[MAX_LENGTH];
    char address[MAX_LENGTH];
    char status[MAX_LENGTH];  // "pending", "ready", "collected"
    struct Order* next;
} Order;

Order* head = NULL;  // Head of the linked list
// Function to create a new order
Order* create_order(int id, const char* customer, const char* phone, const char* address, const char* status) {
    Order* new_order = (Order*)malloc(sizeof(Order));
    new_order->id = id;
    strncpy(new_order->customer, customer, MAX_LENGTH);
    strncpy(new_order->phone, phone, MAX_LENGTH);
    strncpy(new_order->address, address, MAX_LENGTH);
    strncpy(new_order->status, status, MAX_LENGTH);
    new_order->next = NULL;
    return new_order;
}

// Function to add order to the list
void add_order(int id, const char* customer, const char* phone, const char* address, const char* status) {
    Order* new_order = create_order(id, customer, phone, address, status);
    if (head == NULL) {
        head = new_order;
    } else {
        Order* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_order;
    }
}

// Function to load all active orders
void load_orders(FILE* out) {
    Order* temp = head;
    fprintf(out, "[\n");
    while (temp != NULL) {
        fprintf(out, "{\"id\": \"%d\", \"customer\": \"%s\", \"phone\": \"%s\", \"address\": \"%s\", \"status\": \"%s\"},\n", 
                temp->id, temp->customer, temp->phone, temp->address, temp->status);
        temp = temp->next;
    }
    fprintf(out, "]\n");
}

// Function to mark order as ready
void mark_order_ready(int order_id, FILE* out) {
    Order* temp = head;
    while (temp != NULL) {
        if (temp->id == order_id) {
            strncpy(temp->status, "ready", MAX_LENGTH);
            fprintf(out, "{\"status\":\"success\", \"message\":\"Order marked as ready.\"}\n");
            return;
        }
        temp = temp->next;
    }
    fprintf(out, "{\"status\":\"error\", \"message\":\"Order not found.\"}\n");
}

// Function to collect an order (move it to archive)
void collect_order(int order_id, FILE* out) {
    Order* temp = head;
    Order* prev = NULL;
    while (temp != NULL) {
        if (temp->id == order_id) {
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            fprintf(out, "{\"status\":\"success\", \"message\":\"Order collected.\"}\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    fprintf(out, "{\"status\":\"error\", \"message\":\"Order not found.\"}\n");
}

// Function to generate the next order ID (simple approach)
int generate_order_id() {
    static int id = 1;  // Static variable to keep track of the last used ID
    return id++;
}

