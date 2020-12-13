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

  // first, correctly set these 5 variables.
  cache->n_total_cache_line = capacity / block_size;
  cache->n_set = capacity / (assoc * block_size);
  cache->n_offset_bit = log2(block_size);
  cache->n_index_bit = log2(cache->n_set);
  cache->n_tag_bit = ADDRESS_SIZE - cache->n_index_bit - cache->n_offset_bit;

  // next create the cache lines and the array of LRU bits
  // - malloc an array with n_rows
  // - for each element in the array, malloc another array with n_col
  cache->lines = malloc(cache->n_set * sizeof(cache_line_t));
  cache->lru_way = malloc(sizeof(int) * cache->n_set);

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  for (int i = 0; i < cache->n_set; i++)
  {
    cache->lines[i] = malloc(sizeof(cache_line_t) * cache->assoc);
    cache->lru_way[i] = 0;
    for (int j = 0; j < cache->assoc; j++)
    {
      cache->lines[i][j].tag = 0;
      cache->lines[i][j].dirty_f = false;
      cache->lines[i][j].state = INVALID;
    }
  }
  cache->stats = make_cache_stats();
  cache->protocol = protocol;
  return cache;
}

unsigned long get_cache_tag(cache_t *cache, unsigned long addr)
{
  //gets the first n_tag_bit bytes of addr
  unsigned long cache_tag = addr >> (ADDRESS_SIZE - cache->n_tag_bit);
  return cache_tag;
}

unsigned long get_cache_index(cache_t *cache, unsigned long addr)
{
  unsigned long cache_index = (addr >> cache->n_offset_bit) & (cache->n_set - 1);
  return cache_index;
}

unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr)
{
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
  unsigned long tag = get_cache_tag(cache, addr);
  unsigned long index = get_cache_index(cache, addr);

  bool result = MISS;
  bool dirty_evict = false;
  bool upgrade_miss = false;
  int lru = cache->lru_way[index];

  for (int i = 0; i < cache->assoc; i++)
  {
    if (cache->lines[index][i].tag == tag)
    {
      result = HIT && cache->lines[index][i].state != INVALID;
      if (action == LOAD)
      {
        cache->lru_way[index] = (cache->assoc == 2) ? !i : 0;
      }
      else if (action == STORE)
      {
        cache->lines[index][i].dirty_f = true;
        cache->lru_way[index] = (cache->assoc == 2) ? !i : 0;
        if (cache->lines[index][i].state == SHARED)
        {
          result = MISS;
          upgrade_miss = true;
          cache->lines[index][i].state = MODIFIED;
        }
      }
      else if (action == LD_MISS)
      {
        dirty_evict = cache->lines[index][i].dirty_f;
        if (cache->protocol == VI)
        {
          cache->lines[index][i].state = INVALID;
        }
        else if (cache->protocol == MSI)
        {
          cache->lines[index][i].state = cache->lines[index][i].state == INVALID ? INVALID : SHARED;
        }
      }
      else // action == ST_MISS
      {
        dirty_evict = cache->lines[index][i].dirty_f;
        cache->lines[index][i].state = INVALID;
      }
      log_way(i);
      break;
    }
  }
  if (result == MISS && (action == LOAD || action == STORE) && !upgrade_miss)
  {
    cache->lines[index][lru].tag = tag;
    if (cache->protocol == MSI)
    {
      cache->lines[index][lru].state = action == STORE ? MODIFIED : SHARED;
    }
    else
    {
      cache->lines[index][lru].state = VALID;
    }
    dirty_evict = cache->lines[index][lru].dirty_f;
    cache->lines[index][lru].dirty_f = (action == STORE);
    cache->lru_way[index] = (cache->assoc == 2) ? !lru : 0;
    log_way(lru);
  }

  log_set(index);
  update_stats(cache->stats, result, dirty_evict, upgrade_miss, action);
  return result;
}