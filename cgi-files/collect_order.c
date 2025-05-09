// File: collect_order.cgi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *query = getenv("QUERY_STRING");
    char id[100];
    sscanf(query, "id=%s", id);

    FILE *fp = fopen("orders.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *archive = fopen("archive.txt", "a");

    if (fp == NULL || temp == NULL || archive == NULL) {
        printf("Content-Type: application/json\n\n");
        printf("{\"status\":\"File error\"}");
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char currId[100], customer[100], phone[100], address[200], status[100];
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%s", currId, customer, phone, address, status);

        if (strcmp(currId, id) == 0) {
            fprintf(archive, "%s|%s|%s|%s|Collected\n", currId, customer, phone, address);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(fp);
    fclose(temp);
    fclose(archive);

    remove("orders.txt");
    rename("temp.txt", "orders.txt");

    printf("Content-Type: application/json\n\n");
    printf("{\"status\":\"Order Collected\"}");
    return 0;
}
