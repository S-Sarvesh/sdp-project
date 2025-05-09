#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to trim newlines
void trim_newline(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
}

// Reads stdin into buffer
void read_stdin(char *buffer, int size) {
    int c, i = 0;
    while ((c = getchar()) != EOF && i < size - 1) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

// Get value from JSON string by key
char* extract_json_value(const char *json, const char *key) {
    static char value[100];
    char *start = strstr(json, key);
    if (!start) return NULL;
    start = strchr(start, ':');
    if (!start) return NULL;
    start++;
    while (*start == ' ' || *start == '"' || *start == '\'') start++;
    char *end = start;
    while (*end && *end != '"' && *end != '\'' && *end != ',' && *end != '}') end++;
    strncpy(value, start, end - start);
    value[end - start] = '\0';
    return value;
}

int main() {
    char *len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;

    printf("Content-Type: application/json\r\n\r\n");

    if (len <= 0) {
        printf("{\"success\": false, \"message\": \"No data received\"}");
        return 0;
    }

    char input[1024];
    read_stdin(input, sizeof(input));

    char *username = extract_json_value(input, "username");
    char *currentPassword = extract_json_value(input, "currentPassword");
    char *newPassword = extract_json_value(input, "newPassword");

    if (!username || !currentPassword || !newPassword) {
        printf("{\"success\": false, \"message\": \"Invalid input\"}");
        return 0;
    }

    FILE *fp = fopen("/path/to/users.txt", "r");
    if (!fp) {
        printf("{\"success\": false, \"message\": \"Server error (read)\"}");
        return 0;
    }

    char line[256];
    int found = 0;
    FILE *temp = fopen("/tmp/temp_users.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        char fileUser[100], filePass[100];
        sscanf(line, "%[^:]:%s", fileUser, filePass);
        trim_newline(filePass);

        if (strcmp(fileUser, username) == 0) {
            if (strcmp(filePass, currentPassword) != 0) {
                fclose(fp);
                fclose(temp);
                remove("/tmp/temp_users.txt");
                printf("{\"success\": false, \"message\": \"Current password incorrect\"}");
                return 0;
            } else {
                fprintf(temp, "%s:%s\n", username, newPassword);
                found = 1;
            }
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("/path/to/users.txt");
        rename("/tmp/temp_users.txt", "/path/to/users.txt");
        printf("{\"success\": true}");
    } else {
        remove("/tmp/temp_users.txt");
        printf("{\"success\": false, \"message\": \"User not found\"}");
    }

    return 0;
}
