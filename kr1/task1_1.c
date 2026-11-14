#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int day;
    int month;
    int year;
    char* line;
} Line;

static void counting_sort_day(Line* a, Line* tmp, int n) {
    int count[32] = {0};
    for (int i = 0; i<n; i++) {
        count[a[i].day] += 1;
    }
    for (int i = 1; i<32; i++) {
        count[i] += count[i-1];
    }
    for (int i=n-1; i>=0; i--) {
        count[a[i].day]--;
        tmp[count[a[i].day]] = a[i];
    }
    memcpy(a, tmp, n * sizeof(Line));
}

static void counting_sort_month(Line* a, Line* tmp, int n) {
    int count[13] = {0};
    for (int i = 0; i<n; i++) {
        count[a[i].month] += 1;
    }
    for (int i = 1; i<13; i++) {
        count[i] += count[i-1];
    }
    for (int i=n-1; i>=0; i--) {
        count[a[i].month]--;
        tmp[count[a[i].month]] = a[i];
    }
    memcpy(a, tmp, n * sizeof(Line));
}

static void counting_sort_year(Line* a, Line* tmp, int n, int exp) {
    int count[10] = {0};
    for (int i = 0; i<n; i++) {
        int digit = (a[i].year / exp) % 10;
        count[digit]++;
    }
    for (int i=1; i<10; i++) {
        count[i] += count[i-1];
    }
    for (int i=n-1; i>=0; i--) {
        int digit = (a[i].year / exp) % 10;
        count[digit]--;
        tmp[count[digit]] = a[i];
    }
    memcpy(a, tmp, n*sizeof(Line));
}

int main() {
    int capacity = 16;

    Line* a = malloc(sizeof(Line) * capacity);
    if (a == NULL) return 1;

    int n = 0;
    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            continue;
        }

        if (n == capacity) {
            capacity *= 2;

            Line* new_a = realloc(a, sizeof(Line) * capacity);

            if (new_a == NULL) {
                for (int i = 0; i < n; i++) {
                    free(a[i].line);
                }

                free(a);
                return 1;
            }

            a = new_a;
        }

        int len = strlen(buffer);

        while (len > 0 &&
            (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[--len] = '\0';
        }

        if (sscanf(buffer, "%d.%d.%d",
                &a[n].day,
                &a[n].month,
                &a[n].year) != 3) {
            continue;
        }

        a[n].line = malloc((len + 1) * sizeof(char));

        if (a[n].line == NULL) {
            for (int i = 0; i < n; i++) {
                free(a[i].line);
            }

            free(a);
            return 1;
        }

        strcpy(a[n].line, buffer);

        n++;
    }

    Line* tmp = malloc(sizeof(Line) * capacity);
    if (tmp == NULL) {
        free(a);
        return 1;
    }

    // passes: D, M, Y;
    counting_sort_day(a, tmp, n);
    counting_sort_month(a, tmp, n);
    for (int exp=1; exp <= 1000; exp*=10) {
        counting_sort_year(a, tmp, n, exp);
    }

    for (int i = 0; i < n; i++) {
        printf("%s\n", a[i].line);
    }


    free(tmp);
    free(a);
    return 0;
}