#define _POSIX_C_SOURCE 200809L
#include <kv.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define SALT 5381
#define HASH_SHIFT 5
#define EMPTY      NULL
#define TOMBSTONE ((char *)0x1)

kv_t *kv_init(size_t capacity) {
    if (capacity == 0)
        return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL)
        return NULL;

    table->entries = calloc(capacity, sizeof(kv_entry_t));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    return table;
}

size_t hash(const char *key) {
    size_t hash = SALT;
    int c;

    while ((c = *key++))
        hash = ((hash << HASH_SHIFT) + hash) + c;

    return hash;
    /*
    size_t hash = 0x13371337deadbeef;

    while(*key) {
        hash ^= *key;
        hash = hash << 8;
        hash += *key;
        key++;
    }

    return hash;
    */
}

int kv_put(kv_t *db, char *key, char *value) {
    if (db == NULL || key == NULL || value == NULL)
        return KV_ERROR; //-1

    if (db->count == db->capacity)
        return KV_ERROR_FULL; // -2

    size_t index = hash(key) % db->capacity;

    for (size_t i = 0; i < db->capacity; i++) {
        size_t probe = (index + i) % db->capacity;
        kv_entry_t *entry = &db->entries[probe];
        if (entry->key
            && entry->key != (void*)TOMBSTONE
            && !strcmp(entry->key, key)) {
            char *new_value = strdup(value);
            if (new_value == NULL) {
                return KV_ERROR; // -1
            }
            free(entry->value);
            entry->value = new_value;
            return KV_OK;
        }

        if (!entry->key || entry->key == (void*)TOMBSTONE) {
            char *new_key = strdup(key);
            char *new_value = strdup(value);
            if (new_key == NULL || new_value == NULL) {
                free(new_key);
                free(new_value);
                return KV_ERROR; // -1
            }
            entry->key = new_key;
            entry->value = new_value;
            db->count++;
            return KV_OK;
        }
    }
    return KV_ERROR_FULL; // -2
}

char* kv_get(kv_t *db, char *key) {
    if (db == NULL || key == NULL)
        return NULL;

    size_t index = hash(key) %db->capacity;
    for (size_t i = 0; i < db->capacity; i++){
        size_t probe = (index + i) %db->capacity;
        kv_entry_t *entry = &db->entries[probe];

        if (entry->key == NULL)
            return NULL;
        if (entry->key
            && entry->key != (void*)TOMBSTONE
            && !strcmp(key,entry->key)) {
                return entry->value;
            }
    }
    return NULL;

}

int kv_delete(kv_t *db, char *key) {
    if (db == NULL || key == NULL)
        return KV_ERROR;

    size_t index = hash(key) %db->capacity;
    for (size_t i = 0; i < db->capacity; i++){
        size_t probe = (index + i) %db->capacity;
        kv_entry_t *entry = &db->entries[probe];
        if (entry->key == NULL) {
            return KV_ERROR;
        }
        if (entry->key != (void*)TOMBSTONE
            && !strcmp(key, entry->key)) {
                free(entry->key);
                free(entry->value);
                entry->key = TOMBSTONE;
                entry->value = NULL;
                db->count--;
                return KV_OK;
        }
    }
    return KV_ERROR;
}

void kv_free(kv_t *db) {
    if (!db)
        return;

    for (size_t i = 0; i < db->capacity; i++) {
        kv_entry_t *entry = &db->entries[i];
        if (entry->key && entry->key != (void*)TOMBSTONE){
            free(entry->key);
            free(entry->value);
            entry->key = NULL;
            entry->value = NULL;
            db->count--;
        }
    }
    free(db->entries);
    db->entries = NULL;
    db->count = 0;
    free(db);
}
