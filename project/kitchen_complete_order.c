#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define ORDER_FILE "orders.txt"  // Orders file
#define COMPLETED_FILE "completed_orders.txt"  // Completed orders file


typedef struct Order {
    int id;
    char customerName[50];
    char phone[20];
    char location[50];
    char dish[50];
    char status[20];
    struct Order* next;
} Order;


typedef struct Queue {
    Order* front;
    Order* rear;
} Queue;

// Function to create a new order node
Order* createOrder(int id, const char* customerName, const char* phone, const char* location, const char* dish, const char* status) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->id = id;
    strcpy(newOrder->customerName, customerName);
    strcpy(newOrder->phone, phone);
    strcpy(newOrder->location, location);
    strcpy(newOrder->dish, dish);
    strcpy(newOrder->status, status);
    newOrder->next = NULL;
    return newOrder;
}

// Function to initialize an empty queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

// Function to enqueue an order
void enqueue(Queue* q, Order* order) {
    if (q->rear == NULL) {
        q->front = order;
        q->rear = order;
    } else {
        q->rear->next = order;
        q->rear = order;
    }
}

// Function to dequeue an order
Order* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    Order* order = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return order;
}

// Function to load orders from the file and return a queue
Queue* loadOrdersFromFile() {
    FILE* file = fopen(ORDER_FILE, "r");
    if (file == NULL) {
        perror("Unable to open order file");
        return NULL;
    }

    Queue* queue = createQueue();
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        int id;
        char customerName[50], phone[20], location[50], dish[50], status[20];

        // Parsing each order line
        if (sscanf(line, "%d|%49[^|]|%19[^|]|%49[^|]|%49[^|]|%19[^\n]",
                   &id, customerName, phone, location, dish, status) == 6) {
            Order* order = createOrder(id, customerName, phone, location, dish, status);
            enqueue(queue, order);
        }
    }

    fclose(file);
    return queue;
}

// Function to save updated orders back to the file
void saveOrdersToFile(Queue* queue) {
    FILE* file = fopen(ORDER_FILE, "w");
    if (file == NULL) {
        perror("Unable to open order file for writing");
        return;
    }

    Order* current = queue->front;
    while (current) {
        fprintf(file, "%d|%s|%s|%s|%s|%s\n", current->id, current->customerName, current->phone, current->location, current->dish, current->status);
        current = current->next;
    }

    fclose(file);
}

// Function to save completed orders to a separate file
void saveCompletedOrderToFile(Order* order) {
    FILE* file = fopen(COMPLETED_FILE, "a");
    if (file == NULL) {
        perror("Unable to open completed orders file for writing");
        return;
    }

    fprintf(file, "%d|%s|%s|%s|%s|%s\n", order->id, order->customerName, order->phone, order->location, order->dish, order->status);
    fclose(file);
}

// Function to update the status of the first order as completed
void completeOrder(Queue* queue) {
    if (queue->front == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h2>No orders to complete.</h2></body></html>");
        return;
    }

    // Get the first order and update its status
    Order* firstOrder = queue->front;
    strcpy(firstOrder->status, "Completed");

    // Save the updated orders back to the orders.txt file (which is pending orders)
    saveOrdersToFile(queue);

    // Save the completed order to a separate file
    saveCompletedOrderToFile(firstOrder);

    // Output confirmation to the user
    printf("Content-Type: text/html\n\n");
    printf("<html><body><h2>First order completed: Order ID %d</h2></body></html>", firstOrder->id);

    // Free memory allocated for the completed order (it is now marked as completed in the queue)
    dequeue(queue);
    free(firstOrder);
}

int main() {
    // Set content type for the CGI response
    printf("Content-Type: text/html\n\n");

    // Load orders into the queue
    Queue* queue = loadOrdersFromFile();
    if (queue == NULL) {
        printf("<html><body><h2>Error loading orders.</h2></body></html>");
        return 1;
    }

    // Complete the first order
    completeOrder(queue);

    // Free queue memory
    free(queue);
    return 0;
}
