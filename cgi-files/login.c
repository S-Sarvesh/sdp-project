#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_redirect(const char *url) {
    printf("Content-Type: text/html\n");
    printf("Location: %s\n\n", url);
}

int main() {
    char *data = getenv("CONTENT_LENGTH");
    int len = data ? atoi(data) : 0;
    char postData[1024] = {0};
    if (len > 0) {
        fread(postData, 1, len, stdin);
    }

    char role[32], userid[64], password[64];
    sscanf(postData, "role=%[^&]&userid=%[^&]&password=%s", role, userid, password);

    // Decode URL-encoded values
    for (char *p = userid; *p; p++) if (*p == '+') *p = ' ';
    for (char *p = password; *p; p++) if (*p == '+') *p = ' ';

    if (strcmp(role, "admin") == 0) {
        if (strcmp(userid, "admin") == 0 && strcmp(password, "admin@123") == 0) {
            send_redirect("better_adminPage.html");
        } else {
            send_redirect("login.html?error=1");
        }
    } else if (strcmp(role, "kitchen") == 0) {
        FILE *fp = fopen("chefs.txt", "r");
        if (!fp) {
            send_redirect("login.html?error=2");
            return 0;
        }

        char line[128], chefID[64], chefPass[64], found = 0;
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%s %s", chefID, chefPass);
            if (strcmp(chefID, userid) == 0 && strcmp(chefPass, password) == 0) {
                found = 1;
                break;
            }
        }
        fclose(fp);

        if (found) {
            send_redirect("sample.html");
        } else {
            send_redirect("login.html?error=3");
        }
    } else {
        send_redirect("login.html?error=4");
    }

    return 0;
}
