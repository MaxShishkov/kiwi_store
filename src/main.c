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
    printf("value: %s\n", value);

    kv_delete(table, value);
    value = NULL;
    value = kv_get(table, "foo");
    printf("value: %s\n", value);

    kv_free(table);
    table = NULL;

    return 0;
}
