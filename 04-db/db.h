#ifndef DB_H
#define DB_H

#define TABLE_SIZE 128

typedef struct {
    char* key;
    char* value;
} Entry;

typedef struct {
    Entry entries[TABLE_SIZE];
} HashMap;

void hashmap_init(HashMap* map);
void hashmap_set(HashMap* map, const char* key, const char* value);
const char* hashmap_get(HashMap* map, const char* key);

#endif
