#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "chefs.txt" // Text file to store chef data

typedef struct {
    char name[100];
    int age;
    char specialty[100];
    char contact[100];
    int experience;
    char chef_id[50];
    char password[50];
} Chef;

// Function to append a new chef's details to the file
void add_chef_to_file(Chef chef) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Content-type: text/html\n\n");
        printf("<html><body><h2>Error: Unable to open file!</h2></body></html>");
        return;
    }

    // Append chef's details in a specific format
    fprintf(file, "%s|%d|%s|%s|%d|%s|%s\n", chef.name, chef.age, chef.specialty, chef.contact, chef.experience, chef.chef_id, chef.password);
    fclose(file);
}

// Function to handle the form data and store the chef
void add_chef() {
    char name[100], age[10], specialty[100], contact[100], experience[10], chef_id[50], password[50];

    // Reading form data from stdin (using POST data)
    fgets(name, sizeof(name), stdin);
    fgets(age, sizeof(age), stdin);
    fgets(specialty, sizeof(specialty), stdin);
    fgets(contact, sizeof(contact), stdin);
    fgets(experience, sizeof(experience), stdin);
    fgets(chef_id, sizeof(chef_id), stdin);
    fgets(password, sizeof(password), stdin);

    // Strip out newline characters
    name[strcspn(name, "\n")] = 0;
    specialty[strcspn(specialty, "\n")] = 0;
    contact[strcspn(contact, "\n")] = 0;
    experience[strcspn(experience, "\n")] = 0;
    chef_id[strcspn(chef_id, "\n")] = 0;
    password[strcspn(password, "\n")] = 0;

    // Create a new chef object
    Chef new_chef;
    strcpy(new_chef.name, name);
    new_chef.age = atoi(age);
    strcpy(new_chef.specialty, specialty);
    strcpy(new_chef.contact, contact);
    new_chef.experience = atoi(experience);
    strcpy(new_chef.chef_id, chef_id);
    strcpy(new_chef.password, password);

    // Add the chef to the file
    add_chef_to_file(new_chef);

    // Return a success message
    printf("Content-type: text/html\n\n");
    printf("<html><body>");
    printf("<h2>Chef Added Successfully!</h2>");
    printf("<a href=\"admin_dashboard.html\">Back to Dashboard</a>");
    printf("</body></html>");
}

int main() {
    // Call the add_chef function to handle the form data
    add_chef();

    return 0;
}
