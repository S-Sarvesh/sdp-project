#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHEFS 100 // Maximum number of chefs

typedef struct {
    char name[100];
    int age;
    char specialty[100];
    char contact[100];
    int experience;
    char chef_id[50];
    char password[50];
} Chef;

typedef struct {
    Chef chefs[MAX_CHEFS];
    int top; // Stack pointer
} ChefStack;

// Initialize the stack
void init_stack(ChefStack *stack) {
    stack->top = -1;
}

// Push a new chef onto the stack
int push(ChefStack *stack, Chef chef) {
    if (stack->top == MAX_CHEFS - 1) {
        return -1; // Stack is full
    }
    stack->chefs[++(stack->top)] = chef;
    return 0; // Success
}

// Pop a chef from the stack
int pop(ChefStack *stack, Chef *chef) {
    if (stack->top == -1) {
        return -1; // Stack is empty
    }
    *chef = stack->chefs[(stack->top)--];
    return 0; // Success
}

// Retrieve all chefs (for viewing)
int get_chefs(ChefStack *stack, Chef *chefs) {
    if (stack->top == -1) {
        return 0; // No chefs available
    }
    for (int i = 0; i <= stack->top; i++) {
        chefs[i] = stack->chefs[i];
    }
    return stack->top + 1; // Number of chefs
}
