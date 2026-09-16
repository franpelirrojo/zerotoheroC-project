#include <kv.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

    for (size_t i = 0; i < table->capacity; i++) {
        size_t real_idx = (idx + i) % table->capacity;
        kv_entry_t *entry = &table->entries[real_idx];

        if (entry->key &&
                entry->key != (void*)TOMBSTONE
                && !strcmp(entry->key, key)) {
            char *newval = strdup(value);
            if (!newval) return -1;
            free(entry->value);
            entry->value = newval;
            return 0;
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
            return 0;
        }
    }

    return -2;
}

char * kv_get(kv_t *table, char *key) {
    if (!table || !key) return NULL;

    size_t idx = hash(key, table->capacity);

    for (size_t i = 0; i < table->capacity; i++) {
        size_t real_idx = (idx + i) % table->capacity;
        kv_entry_t *entry = &table->entries[real_idx];

        if (!entry->key) return NULL;

        if (entry->key != (void*)TOMBSTONE
                && !strcmp(entry->key, key)) {
            return entry->value;
        }
    }

    return NULL;
}

int kv_delete(kv_t *table, char *key) {
    if (!table || !key) return -1;

    size_t idx = hash(key, table->capacity);

    for (size_t i = 0; i < table->capacity; i++) {
        size_t real_idx = (idx + i) % table->capacity;
        kv_entry_t *entry = &table->entries[real_idx];

        if (!entry->key) return -1;

        if (entry->key == (void*)TOMBSTONE) continue;

        if (!strcmp(entry->key, key)) {
            free(entry->key);
            free(entry->value);
            table->count--;
            entry->key = (void*)TOMBSTONE;
            entry->value = NULL;
            return 0;
        }
    }

    return -1;
}

void kv_free(kv_t *table) {
    for (size_t i = 0; i < table->capacity; i++){
        kv_entry_t *entry = &table->entries[i];
        if (entry->key 
                && entry->key != (void *)TOMBSTONE) {
                    free(entry->key);
                    free(entry->value);
        }
    }

    free(table->entries);
    free(table);
}

void kv_print(kv_t *table) {
    for (size_t i = 0; i < table->capacity; i++){
        if (table->entries[i].key 
                && table->entries[i].key != (void *)TOMBSTONE) {
            printf("[%ld] %s: %s\n", 
                    i,
                    table->entries[i].key,
                    table->entries[i].value);
        }
    }
}
