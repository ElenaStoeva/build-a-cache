#include <stdlib.h>

#include "cache_stats.h"


/* For Project 5 you will consider a variety of cache configurations.
 * For each cache you configure & simulate, you gererate statistics
 * for that cache with an instance of this struct
 */
cache_stats_t *make_cache_stats() {
  cache_stats_t *stats = malloc(sizeof(cache_stats_t));

  stats->total_cpu_accesses = 0;
  stats->total_hits = 0;
  stats->total_stores = 0;
  stats->total_dirty_evics = 0;

  stats->total_bus_snoops = 0;
  stats->total_snoop_hits = 0;

  stats->total_upgrade_miss = 0;
  
  stats->hit_rate = 0.0;

  stats->B_written_to_cache = 0;
  
  stats->B_written_to_bus_wb = 0;  
  stats->B_written_to_bus_wt = 0;
  
  stats->total_traffic_wb = 0;
  stats->total_traffic_wt = 0;

  return stats;
}

/* This code assumes the only actions are LOAD and STORE. 
 * when you move to multi-core you'll need to modify it
 * to correctly update on LD_MISS and ST_MISS actions
 * also need to update total_snoop_hits, total_bus_snoops
*/
void update_stats(cache_stats_t *stats, bool hit_f, bool dirty_evic_f, bool upgrade_miss_f, enum action_t action) {
  if (hit_f)
    stats->total_hits++;
  
  if (action == STORE)
    stats->total_stores++;

  if (dirty_evic_f)
    stats->total_dirty_evics++;
  
  if (upgrade_miss_f)
    stats->total_upgrade_miss++;

  stats->total_cpu_accesses++;
}

// could do this in the previous method, but that's a lot of extra divides...
void calculate_stat_rates(cache_stats_t *stats, int block_size) {

  stats->hit_rate = stats->total_hits / (double)stats->total_cpu_accesses;

  // FIX THIS CODE!
  // you will need to modify this function in order to properly
  // calculate wb and wt data
  stats->B_written_to_cache = 0;
  stats->B_written_to_bus_wb = 0;
  stats->B_written_to_bus_wt = 0;
  stats->total_traffic_wb = 0;
  stats->total_traffic_wt = 0;

}
