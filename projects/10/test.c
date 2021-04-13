#include <stdio.h>

int main() {
    FILE *f = fopen("Square/Square.jack", "r");
    char line[256];
    while (fgets(line, 256, f) != NULL) {
        printf("%s\n", line);
    }
}
