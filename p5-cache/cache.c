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

  // cache_line_t **linesMatrix[cache->n_set][cache->assoc];
  // linesMatrix= (cache_line_t *)malloc(cache->n_set*sizeof(cache_line_t));
  cache->lines = (cache_line_t **)malloc(cache->n_set * sizeof(cache_line_t *));
  cache->lru_way = malloc(sizeof(int) * cache->n_set);

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  // FIX THIS CODE!
  for (int i = 0; i < cache->n_set; i++)
  {
    cache->lines[i] = malloc(sizeof(cache_line_t) * cache->assoc);
    cache->lru_way[i] = 0;
    for (int j = 0; j < cache->assoc; j++)
    {
      // body goes here
      cache->lines[i][j].tag = 0;
      cache->lines[i][j].dirty_f = false;
      cache->lines[i][j].state = INVALID;
    }
  }
  cache->stats = make_cache_stats();
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
  // unsigned long cache_index = (addr << cache->n_tag_bit) >> (cache->n_tag_bit + cache->n_offset_bit);
  unsigned long cache_index = (addr >> cache->n_offset_bit) & (cache->n_set - 1);
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

  int lru_way = cache->lru_way[index];

  bool result = cache->lines[index][lru_way].tag == tag;

  update_stats(cache->stats, result, false, false, action);

   if (!result) {
    cache->lines[index][lru_way].tag = tag;
    cache->lru_way[index] = !lru_way;
  }
return result;

  // // VI protocol
  // // actions: LOAD, STORE, LD_MISS, ST_MISS
  // if ((action == LOAD || action == STORE) && cache->lines[index][lru_way].state == VALID)
  // {
  //   //stay in V
  //   cache->lru_way[index] = !lru_way;
  //   if (cache->lines[index][lru_way].tag == tag)
  //   {
  //     update_stats(cache->stats, true, false, false, action);
  //     return HIT;
  //   }
  //   //otherwise no hit, load data from memory into cache addr and return a miss
  //   cache->lines[index][lru_way].tag = tag;
  //   update_stats(cache->stats, false, false, false, action);
  //   return MISS;
  // }

  // if ((action == LOAD || action == STORE) && cache->lines[index][lru_way].state == INVALID)
  // {
  //   cache->lru_way[index] = !lru_way;
  //   cache->lines[index][lru_way].state = VALID; //transition to V

  //   if (cache->lines[index][lru_way].tag == tag)
  //   {
  //     update_stats(cache->stats, true, false, false, action);
  //     return HIT;
  //   }
  //   //otherwise no hit, load data from memory into cache addr and return a miss
  //   cache->lines[index][lru_way].tag = tag;
  //   update_stats(cache->stats, false, false, false, action);
  //   return MISS;
  // }

  // if ((action == LD_MISS || action == ST_MISS) && cache->lines[index][lru_way].state == VALID)
  // {
  //   cache->lines[index][lru_way].state = INVALID; //transition to I
  //   if (cache->lines[index][lru_way].tag == tag)
  //   {
  //     return HIT;
  //   }
  //   //otherwise no hit, load data from memory into cache addr and return a miss
  //   cache->lines[index][lru_way].tag = tag;
  //   return MISS;
  // }

  // if ((action == LD_MISS || action == ST_MISS) && cache->lines[index][lru_way].state == INVALID)
  // {
  //   //stay in I
  //   if (cache->lines[index][lru_way].tag == tag)
  //   {
  //     return HIT;
  //   }
  //   //otherwise no hit, load data from memory into cache addr and return a miss
  //   cache->lines[index][lru_way].tag = tag;
  //   return MISS;
  // }
  // return MISS;
}
