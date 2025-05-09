// File: get_next_id.cgi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp = fopen("orders.txt", "r");
    int maxId = 0;

    if (fp != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), fp)) {
            char id[100];
            if (sscanf(line, "%[^|]", id) == 1) {
                int num = atoi(id);
                if (num > maxId) {
                    maxId = num;
                }
            }
        }
        fclose(fp);
    }

    printf("Content-Type: application/json\n\n");
    printf("{\"nextOrderId\": \"%d\"}", maxId + 1);
    return 0;
}
