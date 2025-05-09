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

// Function to read all chefs from the file
int get_chefs_from_file(Chef *chefs) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return 0; // No chefs available or file doesn't exist
    }

    int count = 0;
    while (fscanf(file, "%99[^|]|%d|%99[^|]|%99[^|]|%d|%49[^|]|%49[^\n]\n", 
                   chefs[count].name, &chefs[count].age, chefs[count].specialty, chefs[count].contact, 
                   &chefs[count].experience, chefs[count].chef_id, chefs[count].password) != EOF) {
        count++;
    }

    fclose(file);
    return count; // Return the number of chefs read
}

// Function to display chefs
void view_chefs() {
    Chef chefs[100];
    int total_chefs = get_chefs_from_file(chefs);

    printf("Content-type: text/html\n\n");
    printf("<html><body>");

    if (total_chefs == 0) {
        printf("<h2>No chefs available at the moment.</h2>");
    } else {
        printf("<h2>Chef List</h2>");
        for (int i = 0; i < total_chefs; i++) {
            printf("<div class=\"card\">");
            printf("<div class=\"card-body\">");
            printf("<h5 class=\"card-title\">%s</h5>", chefs[i].name);
            printf("<p class=\"card-text\">Specialty: %s</p>", chefs[i].specialty);
            printf("<p class=\"card-text\">Experience: %d years</p>", chefs[i].experience);
            printf("<p class=\"card-text\">Contact: %s</p>", chefs[i].contact);
            printf("<p class=\"card-text\">Chef ID: %s</p>", chefs[i].chef_id);
            printf("</div>");
            printf("</div><br>");
        }
    }

    printf("<a href=\"admin_dashboard.html\">Back to Dashboard</a>");
    printf("</body></html>");
}

int main() {
    // Call the view_chefs function to display the list of chefs
    view_chefs();

    return 0;
}
