#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INGREDIENTS 10

typedef struct {
    int id;
    char name[50];
    int stock;
    int lowStockThreshold;
} Ingredient;

Ingredient ingredients[MAX_INGREDIENTS] = {
    {1, "Flour", 50, 10},
    {2, "Sugar", 20, 5},
    {3, "Eggs", 100, 20},
    {4, "Salt", 15, 5},
    {5, "Butter", 30, 5},
    {6, "Milk", 10, 3},
    {7, "Cheese", 40, 10},
    {8, "Tomato", 25, 10},
    {9, "Lettuce", 12, 3},
    {10, "Chicken", 60, 15}
};

void update_stock(int ingredient_id, int new_stock) {
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        if (ingredients[i].id == ingredient_id) {
            ingredients[i].stock = new_stock;
            break;
        }
    }
}

int main() {
    char *data;
    int ingredient_id, new_stock;
    
    // Get the data from the POST request
    data = getenv("QUERY_STRING");
    if (data != NULL) {
        sscanf(data, "ingredient_id=%d&new_stock=%d", &ingredient_id, &new_stock);
        update_stock(ingredient_id, new_stock);
        
        printf("Content-Type: text/plain\n\n");
        printf("Stock updated successfully!\n");
    } else {
        printf("Content-Type: text/plain\n\n");
        printf("Error: No data received.\n");
    }
    
    return 0;
}
