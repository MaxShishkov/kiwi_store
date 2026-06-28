#include <stdio.h>
#include "kv.h"

#define CAPACITY 1024

int main() {
    kv_t *table = kv_init(CAPACITY);

    printf("table at: %p\n", table);
    printf("table capacity: %zu\n", table->capacity);
    int idx = kv_put(table, "hehe", "haha");
    printf("idx: %d\n", idx);
    idx = kv_put(table, "beep", "boop");
    printf("idx: %d\n", idx);
    free(table);

    return 0;
}
