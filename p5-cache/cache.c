#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cache.h"
#include "print_helpers.h"

// FIX THIS CODE!
void **make_2d_matrix(int n_row, int n_col, size_t size) {
  void **matrix = NULL;

  // malloc an array with n_rows
  // for each element in the array, malloc another array with n_col

  return matrix;
}

cache_t *make_cache(int capacity, int block_size, int assoc, enum protocol_t protocol){
  cache_t *cache = (cache_t *)malloc(sizeof(cache_t));

  cache->capacity = capacity;      // in Bytes
  cache->block_size = block_size;  // in Bytes
  cache->assoc = assoc;            // 1, 2, 3... etc.

  // FIX THIS CODE!
  // first, correctly set these 5 variables. THEY ARE ALL WRONG
  // note: you may find math.h's log2 function useful
  cache->n_total_cache_line = 1;
  cache->n_set = 1;
  cache->n_offset_bit = 1;
  cache->n_index_bit = 1;
  cache->n_tag_bit = 1;

  // next create the cache lines
  // Note; this is also incorrect (it shouldn't be a 1 x 1 cache)
  // FIX THIS CODE!

  cache->lines = (cache_line_t **)make_2d_matrix(1, 1, sizeof(cache_line_t));
  cache->lru_way = (int *)malloc(1 * sizeof(int));

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  // FIX THIS CODE!
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      // body goes here
    }
  }

  return cache;
}

unsigned long get_cache_tag(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  return 0;
}

unsigned long get_cache_index(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  return 0;
}

unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  return 0;
}


/* this method takes a cache, an address, and an action
 * it proceses the cache access. functionality in no particular order: 
 *   - look up the address in the cache, determine if hit or miss
 *   - update the LRU_way, cacheTags, state, dirty flags if necessary
 *   - update the cache statistics (call update_stats)
 * return true if there was a hit, false if there was a miss
 * Use the "get" helper functions above. They make your life easier.
 */
bool access_cache(cache_t *cache, unsigned long addr, enum action_t action) {
  // FIX THIS CODE!


  return true;  // cache hit should return true
}
