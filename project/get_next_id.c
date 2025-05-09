#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int id;
    struct Node* next;
} Node;

void add_id(Node** head, int new_id) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->id = new_id;
    new_node->next = *head;
    *head = new_node;
}

int id_exists(Node* head, int id) {
    Node* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    FILE *fp = fopen("orders.txt", "r");
    Node* id_list = NULL;
    int max_id = 0;

    if (fp != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), fp)) {
            char id_str[100];
            if (sscanf(line, "%[^|]", id_str) == 1) {
                int current_id = atoi(id_str);
                add_id(&id_list, current_id);
                if (current_id > max_id) {
                    max_id = current_id;
                }
            }
        }
        fclose(fp);
    }

    // Find the next available ID (could be max_id + 1 or a gap in the sequence)
    int next_id = max_id + 1;
    
    // Optional: Check for gaps in the ID sequence to reuse deleted IDs
    for (int i = 1; i < max_id; i++) {
        if (!id_exists(id_list, i)) {
            next_id = i;
            break;
        }
    }

    printf("Content-Type: application/json\n\n");
    printf("{\"nextOrderId\": \"%d\"}", next_id);

    free_list(id_list);
    return 0;
}