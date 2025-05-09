#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure for queue
typedef struct ArchiveNode {
    char id[100];
    char customer[100];
    char phone[100];
    char address[200];
    char status[100];
    struct ArchiveNode* next;
} ArchiveNode;

// Queue structure
typedef struct {
    ArchiveNode* front;
    ArchiveNode* rear;
} ArchiveQueue;

// Initialize queue
void initQueue(ArchiveQueue* q) {
    q->front = q->rear = NULL;
}

// Check if queue is empty
int isEmpty(ArchiveQueue* q) {
    return q->front == NULL;
}

// Enqueue a new record
void enqueue(ArchiveQueue* q, ArchiveNode* new_node) {
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    new_node->next = NULL;
}

// Dequeue a record (not used in this example but included for completeness)
ArchiveNode* dequeue(ArchiveQueue* q) {
    if (isEmpty(q)) return NULL;
    
    ArchiveNode* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    return temp;
}

// Free all nodes in the queue
void freeQueue(ArchiveQueue* q) {
    while (!isEmpty(q)) {
        ArchiveNode* temp = dequeue(q);
        free(temp);
    }
}

int main() {
    FILE *fp = fopen("archive.txt", "r");
    ArchiveQueue archive_queue;
    initQueue(&archive_queue);

    printf("Content-Type: application/json\n\n");

    if (fp == NULL) {
        printf("[]");  // Empty JSON array if file not found
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline at end if exists
        line[strcspn(line, "\n")] = 0;

        ArchiveNode* new_node = (ArchiveNode*)malloc(sizeof(ArchiveNode));
        if (!new_node) continue;

        int fields = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%s", 
                          new_node->id, 
                          new_node->customer, 
                          new_node->phone, 
                          new_node->address, 
                          new_node->status);

        if (fields == 5) {
            enqueue(&archive_queue, new_node);
        } else {
            free(new_node);
        }
    }
    fclose(fp);

    // Generate JSON output from the queue
    printf("[");
    ArchiveNode* current = archive_queue.front;
    int first = 1;
    while (current != NULL) {
        if (!first) {
            printf(",");
        }
        printf("{\"id\":\"%s\",\"customer\":\"%s\"}", current->id, current->customer);
        first = 0;
        current = current->next;
    }
    printf("]");

    // Free all allocated memory
    freeQueue(&archive_queue);

    return 0;
}