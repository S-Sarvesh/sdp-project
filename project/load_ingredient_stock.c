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

// Sample ingredients data
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

// Function to generate the stock JSON manually
void generate_stock_json() {
    printf("Content-Type: application/json\n\n");
    printf("[\n");

    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        printf("  {\n");
        printf("    \"id\": %d,\n", ingredients[i].id);
        printf("    \"name\": \"%s\",\n", ingredients[i].name);
        printf("    \"stock\": %d,\n", ingredients[i].stock);
        printf("    \"lowStockThreshold\": %d\n", ingredients[i].lowStockThreshold);
        
        if (i < MAX_INGREDIENTS - 1) {
            printf("  },\n");
        } else {
            printf("  }\n");
        }
    }

    printf("]\n");
}

int main() {
    fflush(stdout);
    generate_stock_json();
    return 0;
}
