#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    FILE *file = fopen("completed_orders.txt", "r");
    if (!file) {
        printf("Content-Type: application/json\n\n[]");
        return 0;
    }

    printf("Content-Type: application/json\n\n");
    printf("[");

    char line[512];
    int first = 1;

    while (fgets(line, sizeof(line), file))
	{
        // Remove newline if present
        line[strcspn(line, "\r\n")] = '\0';

        // Tokenize by '|'
        char *orderID = strtok(line, "|");       // 0
        char *customer = strtok(NULL, "|");      // 1
        char *phone = strtok(NULL, "|");         // 2
        char *address = strtok(NULL, "|");       // 3
        char *dish = strtok(NULL, "|");          // 4
        char *status = strtok(NULL, "|");        // 5
        char *orderTime = strtok(NULL, "|");     // 6
        char *completionTime = strtok(NULL, "|");// 7
        char *notes = strtok(NULL, "|");         // 8

        if (orderID && dish && status && strcmp(status, "Completed") == 0) {
            if (!first) printf(",");
            printf("{");
            printf("\"id\":\"%s\",", orderID);
            printf("\"customer\":\"%s\",", customer ? customer : "");
            printf("\"phone\":\"%s\",", phone ? phone : "");
            printf("\"address\":\"%s\",", address ? address : "");
            printf("\"dish\":\"%s\",", dish ? dish : "");
            printf("\"status\":\"%s\",", status ? status : "");
            printf("\"order_time\":\"%s\",", orderTime ? orderTime : "");
            printf("\"completion_time\":\"%s\",", completionTime ? completionTime : "");
            printf("\"notes\":\"%s\"", notes ? notes : "");
            printf("}");
            first = 0;
        }
    }

    printf("]");
    fclose(file);
    return 0;
}