#include <kv.h>
#include <stdlib.h>
#include <string.h>

#define TOMBSTONE 0x1

size_t hash(char *key, int capacity){
    size_t hash = 0x1337deadF;

    while(*key){
        hash ^= *key;
        hash = hash << 8;
        hash += *key;
        key++;
    }

    return hash % capacity;
}

kv_t * kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t * table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(capacity, sizeof(kv_entry_t));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

int kv_put(kv_t *table, char *key, char *value){
    if (!table || !key || !value) return -1;

    size_t idx = hash(key, table->capacity);

    for (int i = 0; i < table->capacity - 1; i++) {
        size_t real_idx = (idx + i) % table->capacity;
        kv_entry_t *entry = &table->entries[real_idx];

        if (entry->key &&
                entry->key != (void*)TOMBSTONE
                && !strcmp(entry->key, key)) {
            char *newval = strdup(value);
            if (!newval) return -1;
            entry->value = newval;
            return real_idx;
        }

        if (!entry->key || entry->key == (void*)TOMBSTONE) {
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newval || !newkey) {
                free(newval);
                free(newkey);
                return -1;
            }
            entry->value = newval;
            entry->key = newkey;
            table->count++;
            return real_idx;
        }
    }

    return -2;
}

void kv_free(kv_t * table) {
    free(table->entries);
    free(table);
}
