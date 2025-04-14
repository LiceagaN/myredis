#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

unsigned long hash(const char *str){
    unsigned long hash = 5381;
    int c = 0;

    while((c == *str++)) //Loop through each character in the string
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % TABLE_SIZE;// Return the hash value modulo the table size
}

int is_expired(Entry* entry){
    if(entry->expire_at == 0) return 0; // No expiration set{
    return time(NULL) >= entry->expire_at; // Check if the current time is greater than the expiration time
}

void hashmap_init(HashMap* map){
    for(int i = 0; i < TABLE_SIZE; i++){
        map->entries[i].key = NULL; // Initialize all entries to NULL
        map->entries[i].value = NULL;
    }
}

void hashmap_set(HashMap* map, const char* key, const char* value){
    unsigned long idx = hash(key);
    while (map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].key, key) == 0){
            free(map->entries[idx].value); // Free the old value
            map->entries[idx].value = strdup(value); // Duplicate the new value
            return;
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }

    map->entries[idx].key = strdup(key); // Duplicate the key
    map->entries[idx].value = strdup(value); // Duplicate the value
    
}

const char* hashmap_get(HashMap* map, const char* key){
    unsigned long idx = hash(key);
    while(map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].key, key) == 0){
            if(is_expired(&map->entries[idx])) return NULL; // Check if the entry is expired
            return map->entries[idx].value; // Return the value if found
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }
    return NULL; // Return NULL if not found
}

int hashmap_exists(HashMap* map, const char* key){
    unsigned long idx = hash(key);
    while(map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].key, key) == 0){
            if(is_expired(&map->entries[idx])) return 0; // Check if the entry is expired
            return 1;
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }
    return 0; // Return 0 if not found
}

int hashmap_del(HashMap* map, const char* key){
    unsigned long idx = hash(key);
    while(map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].key, key) == 0){
            free(map->entries[idx].key); // Free the key
            free(map->entries[idx].value); // Free the value
            map->entries[idx].key = NULL; // Mark the entry as deleted
            map->entries[idx].value = NULL; // Mark the entry as deleted
            return 1; // Return 1 if deleted successfully
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }
    return 0; // Return 0 if not found
}

int hashmap_expire(HashMap* map, const char* key, int seconds){
    unsigned long idx = hash(key);
    while (map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].key, key) == 0){
            map->entries[idx].expire_at = time(NULL) + seconds; // Set the expiration time
            return 1; // Return 1 if expiration time set successfully
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }
    return 0; // Return 0 if not found
}

int hashmap_ttl(HashMap* map, const char* key){
    unsigned long idx = hash(key);
    while (map->entries[idx].key != NULL){
        if(strcmp(map->entries[idx].value, key) == 0){
            if(map->entries[idx].expire_at == 0) return -1; // No expiration set
            int ttl = map->entries[idx].expire_at - time(NULL); // Calculate the time to live
            return (ttl > 0)  ? ttl : -2; // Return -2 if expired
        }
        idx = (idx + 1) % TABLE_SIZE; // Linear probing
    }
    return -2;   
}