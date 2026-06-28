#include <bits/types/siginfo_t.h>
#include <stdalign.h>
#include <kv.h>
#include <stdlib.h>
#include <string.h>


#define SALT 5381
#define HASH_SHIFT 5
#define EMPTY      NULL
#define TOMBSTONE  ((char *)-1)

kv_t *kv_init(size_t capacity) {
    if (capacity == 0)
        return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL)
        return NULL;

    table->entries = calloc(capacity, sizeof(kv_t));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    return table;
}

size_t hash(char *key) {
    size_t hash = SALT;
    int c;

    while ((c = *key++))
        hash = ((hash << HASH_SHIFT) + hash) + c; /* hash * 33 + c */

    return hash;
}

int kv_put(kv_t *table, char *key, char *value) {
    if (table == NULL || key == NULL || value ==NULL)
        return KV_ERR_NULL;

    if (table->count == table->capacity)
        return KV_ERR_CAPACITY;

    size_t index = hash(key) % table->capacity;

    for (int i = 0; i < table->capacity; i++) {
        size_t probe = (index + i) % table->capacity;
        kv_entry_t *entry = &table->entries[probe];
        if (entry->key && entry->key != TOMBSTONE &&
            !strcmp(entry->key, key)) {
            char *new_value = strdup(value);
            if (new_value == NULL) {
                free(new_value);
                return KV_ERR_MEMORY;
            }
            free(entry->value);
            entry->value = new_value;
            return probe;
        }

        if (!entry->key || entry->key == TOMBSTONE) {
            char *new_key = strdup(key);
            char *new_value = strdup(value);
            if (new_key == NULL || new_value == NULL) {
                free(new_key);
                free(new_value);
                return KV_ERR_MEMORY;
            }
            entry->key = new_key;
            entry->value = new_value;
            table->count++;
            return probe;
        }
    }
    return KV_ERROR;
}
