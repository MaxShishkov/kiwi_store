#include <stdio.h>
#include "kv.h"

#define CAPACITY 5

int main() {
    kv_t *table = kv_init(CAPACITY);

    printf("table at: %p\n", table);
    printf("table capacity: %zu\n", table->capacity);
    free(table);

    return 0;
}
