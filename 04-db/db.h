#ifndef DB_H
#define DB_H

#define TABLE_SIZE 128
#include <time.h>

typedef struct {
    char* key;
    char* value;
    time_t expire_at; // Timestamp for the entry
} Entry;

typedef struct {
    Entry entries[TABLE_SIZE];
} HashMap;

void hashmap_init(HashMap* map);
void hashmap_set(HashMap* map, const char* key, const char* value);
const char* hashmap_get(HashMap* map, const char* key);
int hashmap_del(HashMap* map, const char* key);
int hashmap_exists(HashMap* map, const char* key);
int hashmap_expire(HashMap* map, const char* key, int seconds);
int hashmap_ttl(HashMap* map, const char* key);



#endif
