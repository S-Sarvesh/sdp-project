#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define FILENAME "admin_messages.txt"

int main() {
    char *data = getenv("CONTENT_LENGTH");
    int len = data ? atoi(data) : 0;
    char input[MAX] = {0};
    
    if (len > 0) {
        fread(input, 1, len, stdin);
        char *msg = strstr(input, "message=");
        if (msg) {
            msg += 8; // move past 'message='
            for (int i = 0; msg[i]; i++) {
                if (msg[i] == '+') msg[i] = ' ';
                if (msg[i] == '%') {
                    int hex;
                    sscanf(msg + i + 1, "%2x", &hex);
                    msg[i] = (char)hex;
                    memmove(msg + i + 1, msg + i + 3, strlen(msg + i + 3) + 1);
                }
            }

            FILE *fp = fopen(FILENAME, "a");
            if (fp != NULL) {
                fprintf(fp, "%s\n", msg);
                fclose(fp);
            }
        }
    }

    printf("Content-Type: text/plain\r\n\r\n");
    printf("Message received.");
    return 0;
}
