#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* fptr = fopen("test.txt", "r");
    char line[256];
    int result = 0;
    int a = 0, b = 0;
    bool flag = true;
    bool decreasing = true;

    if (fptr != NULL) {
        while (fgets(line, sizeof(line), fptr)) {
            sscanf(line, "%d %d", &a, &b);
            flag = true;
            if (abs(a - b) > 3 || a == b) flag = false;
            a = b;
            decreasing = (b < a);

            while (sscanf(line, "%d", &b)) {
                if (b == a) flag = false;
                if (decreasing == true && a > b) flag = false;
                if (decreasing == false && b > a) flag = false;

                b = a;
            }

            if (flag == true) result++;
        }
    }

    printf("%d\n", result);
    return 0;
}
