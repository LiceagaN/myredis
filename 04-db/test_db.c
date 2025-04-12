#include <stdio.h>
#include <stdlib.h>
#include "db.h"

int main(){
    HashMap db;
    hashmap_init(&db);

    hashmap_set(&db, "foo", "bar");
    hashmap_set(&db, "hello", "world");

    printf("foo => %s\n", hashmap_get(&db, "foo"));
    printf("hello => %s\n", hashmap_get(&db, "hello"));
    printf("nonexistent => %s\n", hashmap_get(&db, "nonexistent"));
    // Free allocated memory
    for(int i = 0; i < TABLE_SIZE; i++){
        free(db.entries[i].key);
        free(db.entries[i].value);
    }  
    return 0;
}