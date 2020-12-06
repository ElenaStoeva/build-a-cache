#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cache.h"
#include "print_helpers.h"

cache_t *make_cache(int capacity, int block_size, int assoc, enum protocol_t protocol)
{
  cache_t *cache = malloc(sizeof(cache_t));

  cache->capacity = capacity;     // in Bytes
  cache->block_size = block_size; // in Bytes, size of a single block
  cache->assoc = assoc;           // 1, 2, 3... etc.

  // FIX THIS CODE!
  // first, correctly set these 5 variables. THEY ARE ALL WRONG
  // note: you may find math.h's log2 function useful
  cache->n_total_cache_line = capacity / block_size;
  cache->n_set = capacity / (assoc * block_size);
  cache->n_offset_bit = log2(block_size);
  cache->n_index_bit = log2(cache->n_set);
  cache->n_tag_bit = 32 - cache->n_index_bit - cache->n_offset_bit;

  // next create the cache lines and the array of LRU bits
  // - malloc an array with n_rows
  // - for each element in the array, malloc another array with n_col
  // FIX THIS CODE!

  cache_line_t **linesMatrix[cache->n_set][cache->assoc];
  //linesMatrix= (cache_line_t *)malloc(sizeof(cache_line_t));
  cache->lines = linesMatrix;
  int lruArray[cache->n_set];
  cache->lru_way = lruArray;

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  // FIX THIS CODE!
  for (int i = 0; i < cache->n_set; i++)
  {
    cache->lru_way[i] = 0;
    for (int j = 0; j < cache->assoc; j++)
    {
      // body goes here
      cache_line_t *line = malloc(sizeof(cache_line_t));
      line->tag = 0;
      line->dirty_f = false;
      line->state = INVALID;
      cache->lines[i][j] = *line;
    }
  }

  return cache;
}

unsigned long get_cache_tag(cache_t *cache, unsigned long addr)
{
  // FIX THIS CODE!
  //gets the first n_tag_bit bytes of addr
  unsigned long cache_tag = addr >> (32 - cache->n_tag_bit);
  return cache_tag;
}

unsigned long get_cache_index(cache_t *cache, unsigned long addr)
{
  // FIX THIS CODE!
  //pi's note: this doesn't give the right output but seems logically correct??
  unsigned long cache_index = (addr << cache->n_tag_bit) >> (cache->n_tag_bit + cache->n_offset_bit);
  return cache_index;
}

unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr)
{
  // FIX THIS CODE!
  unsigned long cache_block_addr = (addr >> cache->n_offset_bit) << cache->n_offset_bit;
  return cache_block_addr;
}

/* this method takes a cache, an address, and an action
 * it proceses the cache access. functionality in no particular order: 
 *   - look up the address in the cache, determine if hit or miss
 *   - update the LRU_way, cacheTags, state, dirty flags if necessary
 *   - update the cache statistics (call update_stats)
 * return true if there was a hit, false if there was a miss
 * Use the "get" helper functions above. They make your life easier.
 */
bool access_cache(cache_t *cache, unsigned long addr, enum action_t action)
{
  // FIX THIS CODE!
  unsigned long tag = get_cache_tag(cache, addr);
  unsigned long index = get_cache_index(cache, addr);
  // unsigned long block_addr = get_cache_block_addr(cache, addr);

  // for direct-mapped
  if (cache->lines[index][0].tag == tag && cache->lines[index][0].state == VALID)
  {
    return HIT;
  }

  //otherwise no hit, load data from memory into cache addr and return a miss
  cache->lines[index][0].tag = tag;
  cache->lines[index][0].state = VALID;
  return MISS;

  // return true; // cache hit should return true
}
