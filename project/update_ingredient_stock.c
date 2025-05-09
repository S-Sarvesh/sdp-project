#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct IngredientNode {
    int id;
    char name[50];
    int stock;
    int lowStockThreshold;
    struct IngredientNode* next;
} IngredientNode;

// Global head pointer for the linked list
IngredientNode* ingredients_head = NULL;

// Function to initialize the ingredients linked list
void initialize_ingredients() {
    // Create initial ingredients
    IngredientNode* initial_ingredients[] = {
        &(IngredientNode){1, "Flour", 50, 10, NULL},
        &(IngredientNode){2, "Sugar", 20, 5, NULL},
        &(IngredientNode){3, "Eggs", 100, 20, NULL},
        &(IngredientNode){4, "Salt", 15, 5, NULL},
        &(IngredientNode){5, "Butter", 30, 5, NULL},
        &(IngredientNode){6, "Milk", 10, 3, NULL},
        &(IngredientNode){7, "Cheese", 40, 10, NULL},
        &(IngredientNode){8, "Tomato", 25, 10, NULL},
        &(IngredientNode){9, "Lettuce", 12, 3, NULL},
        &(IngredientNode){10, "Chicken", 60, 15, NULL}
    };
    
    // Link them together
    for (int i = 0; i < 9; i++) {
        initial_ingredients[i]->next = initial_ingredients[i+1];
    }
    
    ingredients_head = initial_ingredients[0];
}

// Function to update stock for an ingredient
void update_stock(int ingredient_id, int new_stock) {
    IngredientNode* current = ingredients_head;
    while (current != NULL) {
        if (current->id == ingredient_id) {
            current->stock = new_stock;
            return;
        }
        current = current->next;
    }
}

// Function to free the linked list
void free_ingredients() {
    IngredientNode* current = ingredients_head;
    while (current != NULL) {
        IngredientNode* temp = current;
        current = current->next;
        free(temp);
    }
    ingredients_head = NULL;
}

int main() {
    // Initialize the ingredients linked list
    initialize_ingredients();
    
    char *data;
    int ingredient_id, new_stock;
    
    // Get the data from the POST request
    data = getenv("QUERY_STRING");
    if (data != NULL) {
        if (sscanf(data, "ingredient_id=%d&new_stock=%d", &ingredient_id, &new_stock) == 2) {
            update_stock(ingredient_id, new_stock);
            
            printf("Content-Type: text/plain\n\n");
            printf("Stock updated successfully!\n");
        } else {
            printf("Content-Type: text/plain\n\n");
            printf("Error: Invalid data format.\n");
        }
    } else {
        printf("Content-Type: text/plain\n\n");
        printf("Error: No data received.\n");
    }
    
    // Free the linked list
    free_ingredients();
    
    return 0;
}