#ifndef KV_H
#define KV_H

#include <stdlib.h>

typedef enum {
    KV_OK = 0,
    KV_ERROR = -1,
    KV_ERR_NULL = -2,
    KV_ERR_CAPACITY = -3,
    KV_ERR_MEMORY = -4,
} KvStatus;

typedef struct {
    char *key;
    char *value;
} kv_entry_t;

typedef struct {
    size_t capacity;
    size_t count;
    kv_entry_t *entries;
} kv_t;

kv_t *kv_init(size_t capacity);
int kv_put(kv_t *table, char *key, char *value);
#endif
