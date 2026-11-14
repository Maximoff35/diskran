#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    unsigned int day;
    unsigned int month;
    uint32_t year;
    char *line;
} Item;

static void counting_sort_day(Item *a, Item *tmp, size_t n) {
    size_t count[32] = {0};

    for (size_t i = 0; i < n; ++i) {
        ++count[a[i].day];
    }

    for (size_t i = 1; i < 32; ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = n; i > 0; --i) {
        unsigned int key = a[i - 1].day;
        tmp[--count[key]] = a[i - 1];
    }

    memcpy(a, tmp, n * sizeof(Item));
}

static void counting_sort_month(Item *a, Item *tmp, size_t n) {
    size_t count[13] = {0};

    for (size_t i = 0; i < n; ++i) {
        ++count[a[i].month];
    }

    for (size_t i = 1; i < 13; ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = n; i > 0; --i) {
        unsigned int key = a[i - 1].month;
        tmp[--count[key]] = a[i - 1];
    }

    memcpy(a, tmp, n * sizeof(Item));
}

static void counting_sort_year_byte(
    Item *a,
    Item *tmp,
    size_t n,
    unsigned int shift
) {
    size_t count[256] = {0};

    for (size_t i = 0; i < n; ++i) {
        unsigned int byte = (a[i].year >> shift) & 255u;
        ++count[byte];
    }

    for (size_t i = 1; i < 256; ++i) {
        count[i] += count[i - 1];
    }

    for (size_t i = n; i > 0; --i) {
        unsigned int byte = (a[i - 1].year >> shift) & 255u;
        tmp[--count[byte]] = a[i - 1];
    }

    memcpy(a, tmp, n * sizeof(Item));
}

static void radix_sort(Item *a, size_t n) {
    if (n < 2) {
        return;
    }

    Item *tmp = malloc(n * sizeof(Item));

    if (tmp == NULL) {
        exit(EXIT_FAILURE);
    }

    // passes: D, M, Y;
    counting_sort_day(a, tmp, n);
    counting_sort_month(a, tmp, n);

    for (unsigned int shift = 0; shift < 32; shift += 8) {
        counting_sort_year_byte(a, tmp, n, shift);
    }

    free(tmp);
}

int main(void) {
    size_t capacity = 16;
    size_t size = 0;

    Item *items = malloc(capacity * sizeof(Item));

    if (items == NULL) {
        return EXIT_FAILURE;
    }

    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);

        while (len > 0 &&
               (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[--len] = '\0';
        }

        if (len == 0) {
            continue;
        }

        unsigned int day;
        unsigned int month;
        uint32_t year;

        if (sscanf(buffer, "%u.%u.%u", &day, &month, &year) != 3) {
            continue;
        }

        if (size == capacity) {
            capacity *= 2;

            Item *new_items =
                realloc(items, capacity * sizeof(Item));

            if (new_items == NULL) {
                for (size_t i = 0; i < size; ++i) {
                    free(items[i].line);
                }

                free(items);
                return EXIT_FAILURE;
            }

            items = new_items;
        }

        items[size].day = day;
        items[size].month = month;
        items[size].year = year;

        items[size].line = malloc(len + 1);

        if (items[size].line == NULL) {
            for (size_t i = 0; i < size; ++i) {
                free(items[i].line);
            }

            free(items);
            return EXIT_FAILURE;
        }

        memcpy(items[size].line, buffer, len + 1);

        ++size;
    }

    radix_sort(items, size);

    for (size_t i = 0; i < size; ++i) {
        puts(items[i].line);
        free(items[i].line);
    }

    free(items);

    return 0;
}