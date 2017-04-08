/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Hash map.
 */

#ifndef SL_HASHMAP_H_
#define SL_HASHMAP_H_

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A hash map. */
typedef struct sl_hashmap sl_hashmap;

/**
 * Creates a new hash map. Returns NULL if memory allocation fails.
 *
 * @param initialCapacity number of expected entries
 * @param hash function which hashes keys
 * @param equals function which compares keys for equality
 */
sl_hashmap* sl_hashmap_create(size_t initialCapacity,
        int (*hash)(void* key), bool (*equals)(void* keyA, void* keyB),
        void (*freekey)(void *key));

/**
 * Frees the hash map. Does not free the keys or values themselves.
 */
void sl_hashmap_destroy(sl_hashmap* map);

/**
 * Hashes the memory pointed to by key with the given size. Useful for
 * implementing hash functions.
 */
int sl_hashmap_hash(void* key, size_t keySize);

/**
 * Puts value for the given key in the map. Returns pre-existing value if
 * any.
 *
 * If memory allocation fails, this function returns NULL, the map's size
 * does not increase, and errno is set to ENOMEM.
 */
void* sl_hashmap_put(sl_hashmap* map, void* key, void* value);

/**
 * Gets a value from the map. Returns NULL if no entry for the given key is
 * found or if the value itself is NULL.
 */
void* sl_hashmap_get(sl_hashmap* map, void* key);

/**
 * Returns true if the map contains an entry for the given key.
 */
bool sl_hashmap_containsKey(sl_hashmap* map, void* key);

/**
 * Gets the value for a key. If a value is not found, this function gets a 
 * value and creates an entry using the given callback.
 *
 * If memory allocation fails, the callback is not called, this function
 * returns NULL, and errno is set to ENOMEM.
 */
void* sl_hashmap_memoize(sl_hashmap* map, void* key,
        void* (*initialValue)(void* key, void* context), void* context);

/**
 * Removes an entry from the map. Returns the removed value or NULL if no
 * entry was present.
 */
void* sl_hashmap_remove(sl_hashmap* map, void* key);

/**
 * Gets the number of entries in this map.
 */
size_t sl_hashmap_size(sl_hashmap* map);

/**
 * Invokes the given callback on each entry in the map. Stops iterating if
 * the callback returns false.
 */
void sl_hashmap_forEach(sl_hashmap* map,
        bool (*callback)(void* key, void* value, void* context),
        void* context);

///**
// * Concurrency support.
// */
//
///**
// * Locks the hash map so only the current thread can access it.
// */
//void sl_hashmap_lock(sl_hashmap* map);
//
///**
// * Unlocks the hash map so other threads can access it.
// */
//void sl_hashmap_unlock(sl_hashmap* map);

/**
 * Key utilities.
 */

/**
 * Hashes int keys. 'key' is a pointer to int.
 */
int sl_hashmap_intHash(void* key);

/**
 * Compares two int keys for equality.
 */
bool sl_hashmap_intEquals(void* keyA, void* keyB);

/**
 * Duplicate Int, returning an identical malloc'd int pointer.
 */
int* sl_hashmap_intdup(int i);

/**
 * Frees the key, while sl_hashmap_remove the value.
 */
void sl_hashmap_freekey(void *key);

/**
 * For debugging.
 */

/**
 * Gets current capacity.
 */
size_t sl_hashmap_currentCapacity(sl_hashmap* map);

/**
 * Counts the number of entry collisions.
 */
size_t sl_hashmap_countCollisions(sl_hashmap* map);

#ifdef __cplusplus
}
#endif

#endif /* SL_HASHMAP_H_ */
