#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

int main() {
    printf("Content-Type: application/json\n\n");

    // Step 1: Get POST data (assumes "id=xxx" is posted as form data)
    char *content_length = getenv("CONTENT_LENGTH");
    int length = content_length ? atoi(content_length) : 0;

    if (length == 0) {
        printf("{\"status\": \"error\", \"message\": \"No data received\"}");
        return 1;
    }

    char input[MAX_LINE];
    fgets(input, length + 1, stdin);  // Read POST body

    // Parse the order ID from POST data (e.g., id=123)
    char *query = strstr(input, "id=");
    if (!query) {
        printf("{\"status\": \"error\", \"message\": \"Invalid query\"}");
        return 1;
    }

    int target_id = atoi(query + 3); // Extract ID value
    if (target_id <= 0) {
        printf("{\"status\": \"error\", \"message\": \"Invalid order ID\"}");
        return 1;
    }

    // Step 2: Open Files
    FILE *fp = fopen("orders.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *archive = fopen("archive.txt", "a");

    if (!fp || !temp || !archive) {
        printf("{\"status\": \"error\", \"message\": \"Error opening files\"}");
        return 1;
    }

    char line[MAX_LINE];
    int found = 0;

    // Step 3: Process Each Line
    while (fgets(line, sizeof(line), fp)) {
        int id;
        char name[50], phone[20], location[50], item[50], status[20];

        // Make a copy since strtok will modify it
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);

        // Tokenize by '|'
        char *token = strtok(line_copy, "|");
        if (!token) continue;
        id = atoi(token);

        if (id == target_id) {
            // Order matched — read remaining fields
            token = strtok(NULL, "|"); if (!token) continue; strcpy(name, token);
            token = strtok(NULL, "|"); if (!token) continue; strcpy(phone, token);
            token = strtok(NULL, "|"); if (!token) continue; strcpy(location, token);
            token = strtok(NULL, "|"); if (!token) continue; strcpy(item, token);

            // Skip status
            fprintf(temp, "%d|%s|%s|%s|%s|Cancelled\n", id, name, phone, location, item);
            fprintf(archive, "%d|%s|%s|%s|%s|Cancelled\n", id, name, phone, location, item);
            found = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    fclose(archive);

    // Replace original file
    remove("orders.txt");
    rename("temp.txt", "orders.txt");

    if (found) {
        printf("{\"status\": \"success\", \"message\": \"Order with ID %d cancelled successfully.\"}", target_id);
    } else {
        printf("{\"status\": \"error\", \"message\": \"Order ID %d not found.\"}", target_id);
    }

    return 0;
}
