#include <stdio.h>
#include "kv.h"

#define CAPACITY 1024

int main() {
    kv_t *table = kv_init(CAPACITY);

    printf("table at: %p\n", table);
    printf("table capacity: %zu\n", table->capacity);

    kv_put(table, "foo", "foo");
    kv_put(table, "bar", "bar");
    kv_put(table, "beep", "beep");
    char *value = kv_get(table, "foo");
    //char *value2 = kv_get(table, "beep");
    //char *value3 = kv_get(table, "not there");
    printf("value: %s\n", value);
    //printf("value2: %s\n", value2);
    //printf("val3: %s\n", value3);

    free(table);

    return 0;
}
