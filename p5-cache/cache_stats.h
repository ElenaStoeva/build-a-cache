#ifndef __CACHE_STATS_H
#define __CACHE_STATS_H

#include <stdbool.h>
#include <stdio.h>

enum action_t { LOAD, STORE, LD_MISS, ST_MISS };

typedef struct {
  long total_cpu_accesses;
  long total_hits;
  long total_stores;
  long total_dirty_evics;

  long total_bus_snoops; // how many times did you snoop an event from another core?
  long total_snoop_hits; // how many times did a bus event occur for a valid cache
	                 // line you had in your cache?
  long total_upgrade_miss;
  
  double hit_rate;

  long B_written_to_cache;  

  long B_written_to_bus_wb;  // write-back
  long B_written_to_bus_wt;  // write-thru

  long total_traffic_wb;  // write-back
  long total_traffic_wt;  // write-thru

  } cache_stats_t;

cache_stats_t *make_cache_stats();
void calculate_stat_rates(cache_stats_t *stats, int block_size);
void update_stats(cache_stats_t *stats, bool hit_f, bool dirty_evic_f, bool upgrade_miss_f, enum action_t action);

#endif  // CACHE_STATS
