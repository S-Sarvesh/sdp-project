// File: load_archive.cgi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp = fopen("archive.txt", "r");

    printf("Content-Type: application/json\n\n");

    if (fp == NULL) {
        printf("[]");  // Empty JSON array if file not found
        return 0;
    }

    printf("[");
    int first = 1;
    char line[1024];

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline at end if exists
        line[strcspn(line, "\n")] = 0;

        char id[100], customer[100], phone[100], address[200], status[100];

        int fields = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%s", id, customer, phone, address, status);

        if (fields == 5) {
            if (!first) {
                printf(",");
            }
            printf("{\"id\":\"%s\",\"customer\":\"%s\"}", id, customer);
            first = 0;
        }
    }

    printf("]");

    fclose(fp);
    return 0;
}
