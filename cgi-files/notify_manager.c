#include <stdio.h>
#include <stdlib.h>

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

void send_low_stock_notification() {
    printf("Content-Type: text/plain\n\n");
    printf("Low stock alert sent to manager.\n");
    for (int i = 0; i < MAX_INGREDIENTS; i++) {
        if (ingredients[i].stock <= ingredients[i].lowStockThreshold) {
            printf("Low stock ingredient: %s (Stock: %d)\n", ingredients[i].name, ingredients[i].stock);
        }
    }
}

int main() {
    send_low_stock_notification();
    return 0;
}
