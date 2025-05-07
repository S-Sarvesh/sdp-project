#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100

// Define the order structure
typedef struct Order {
    char id[10];
    char customer_name[50];
    char phone_number[20];
    char address[100];
    char dish[50];
    char status[20];
} Order;

// Queue structure
typedef struct Queue {
    Order orders[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
void initQueue(Queue* queue) {
    queue->front = 0;
    queue->rear = -1;
}

// Function to check if the queue is empty
int isQueueEmpty(Queue* queue) {
    return queue->front > queue->rear;
}

// Function to add an order to the queue
void enqueue(Queue* queue, Order order) {
    if (queue->rear < MAX_QUEUE_SIZE - 1) {
        queue->orders[++queue->rear] = order;
    } else {
        printf("Queue is full!\n");
    }
}

// Function to remove the first order from the queue (dequeue)
Order dequeue(Queue* queue) {
    if (!isQueueEmpty(queue)) {
        return queue->orders[queue->front++];
    } else {
        printf("Queue is empty!\n");
        exit(1);
    }
}

// Function to load orders from the file into the queue
void loadOrdersFromFile(Queue* queue) {
    FILE *orders_file = fopen("orders.txt", "r");
    if (!orders_file) {
        perror("Error opening orders file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), orders_file)) {
        Order order;
        char *token = strtok(line, "|");
        strcpy(order.id, token);

        token = strtok(NULL, "|");
        strcpy(order.customer_name, token);

        token = strtok(NULL, "|");
        strcpy(order.phone_number, token);

        token = strtok(NULL, "|");
        strcpy(order.address, token);

        token = strtok(NULL, "|");
        strcpy(order.dish, token);

        token = strtok(NULL, "|");
        strcpy(order.status, token);

        enqueue(queue, order);
    }

    fclose(orders_file);
}

// Function to write updated orders back to the file
void writeOrdersToFile(Queue* queue) {
    FILE *orders_file = fopen("orders.txt", "w");
    if (!orders_file) {
        perror("Error opening orders file");
        return;
    }

    for (int i = queue->front; i <= queue->rear; i++) {
        fprintf(orders_file, "%s|%s|%s|%s|%s|%s\n", 
            queue->orders[i].id, 
            queue->orders[i].customer_name, 
            queue->orders[i].phone_number, 
            queue->orders[i].address, 
            queue->orders[i].dish, 
            queue->orders[i].status);
    }

    fclose(orders_file);
}

// Function to move a completed order to the completed_orders file
void moveToCompletedOrders(Order order) {
    FILE *completed_file = fopen("completed_orders.txt", "a");
    if (!completed_file) {
        perror("Error opening completed orders file");
        return;
    }

    fprintf(completed_file, "%s|%s|%s|%s|%s|%s\n", 
        order.id, 
        order.customer_name, 
        order.phone_number, 
        order.address, 
        order.dish, 
        order.status);

    fclose(completed_file);
}

int main() {
    Queue queue;
    initQueue(&queue);

    // Load orders from the file into the queue
    loadOrdersFromFile(&queue);

    // If the queue is not empty, complete the first order
    if (!isQueueEmpty(&queue)) {
        // Dequeue the first order
        Order first_order = dequeue(&queue);
        
        // Mark the order as completed
        strcpy(first_order.status, "Completed");

        // Move the completed order to the completed orders file
        moveToCompletedOrders(first_order);

        // Write the remaining orders back to the orders.txt file
        writeOrdersToFile(&queue);

        // Print success message
        printf("Order %s completed successfully.\n", first_order.id);
    } else {
        printf("Error: No orders in the queue.\n");
    }

    return 0;
}
