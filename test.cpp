#include <stdio.h>

int main() {
    FILE *f = fopen("output.html", "w");
    if (!f) return 1;

    fprintf(f, "<!DOCTYPE html><html><body>\n");
    fprintf(f, "<p style='color:red;'>Hello Red!</p>\n");
    fprintf(f, "<p style='color:green;'>Hello Green!</p>\n");
    fprintf(f, "<p style='color:blue;'>Hello Blue!</p>\n");
    fprintf(f, "</body></html>\n");

    fclose(f);
    return 0;
}
