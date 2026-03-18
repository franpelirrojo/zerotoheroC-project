#include <stdio.h>
#include <kv.h>

int main(int argc, char *argv[])
{
    kv_t *table = kv_init(5);
    printf("%p\n", table);
    printf("%ld\n", table->capacity);
    printf("%p\n", table->entries);
    return EXIT_SUCCESS;
}
