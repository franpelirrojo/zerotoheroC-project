#include <stdio.h>
#include <kv.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int size = 1024;
    kv_t *table = kv_init(size);

    assert(table != NULL);
    assert(table->capacity == size);
    assert(table->count == 0);
    
    kv_put(table, "hehe", "foo");
    kv_put(table, "hehe", "boo");
    kv_put(table, "haha", "foo");
    kv_put(table, "Para nada es así", "joo");
    kv_put(table, "jaja", "poo");

    for (int i = 0; i < table->capacity; i++){
        if (table->entries[i].key) {
            printf("[%d] %s: %s\n", 
                    i,
                    table->entries[i].key,
                    table->entries[i].value);
        }
    }

    kv_put(table, "name", "Dolly");
    assert(!strcmp(kv_get(table, "name"), "Dolly"));
    assert(kv_get(table, "miss") == NULL);

    return EXIT_SUCCESS;
}
