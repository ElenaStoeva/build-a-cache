#include <stdio.h>

#include "cache.h"
#include "cache_stats.h"
#include "simulator.h"
#include "print_helpers.h"


/* fields you might want to have print */
int print_set = 0;
int print_way = 0;


void log_set(int set) {
  print_set = set;
}

void log_way(int way) {
  print_way = way;
}


void print_simulator_header(simulator_t *sim) {
  printf("P5 Printout for CS 3410\n");
  printf("----------------------------------\n");

  printf("Trace  \t\t%s\n", sim->trace);
  printf("Instruction Limit \t");
  if (sim->limit_insn) {
    printf("%d\n", sim->insn_limit);
  } else {
    printf("none\n");
  }
  print_cache_config(sim->cache[0]); // caches must be identical, so [0] is fine
}

void print_stats(cache_stats_t *stats, int core) {
  printf("%d.total_cpu_accesses \t%ld\n", core, stats->total_cpu_accesses);
  printf("%d.total_loads \t\t%ld\n", core, stats->total_cpu_accesses - stats->total_stores);
  printf("%d.total_stores \t\t%ld\n", core, stats->total_stores);
  printf("%d.total_hits \t\t%ld\n", core, stats->total_hits);
  printf("%d.total_misses \t\t%ld\n", core, stats->total_cpu_accesses - stats->total_hits);
  printf("%d.hit_rate \t\t%.2f\n", core, stats->hit_rate * 100.0);
  printf("%d.miss_rate \t\t%.2f\n", core, (1 - stats->hit_rate) * 100.0);
  printf("%d.total_upgrade_miss \t%ld\n", core, stats->total_upgrade_miss);
  printf("%d.total_bus_snoops \t%ld\n", core, stats->total_bus_snoops);
  printf("%d.total_snoop_hits \t%ld\n", core, stats->total_snoop_hits);
  printf("%d.total_dirty_evics \t%ld\n", core, stats->total_dirty_evics);
  printf("%d.B_written_to_cache \t%ld\n", core, stats->B_written_to_cache);
  printf("%d.B_written_to_bus_wb \t%ld\n", core, stats->B_written_to_bus_wb);
  printf("%d.B_written_to_bus_wt \t%ld\n", core, stats->B_written_to_bus_wt);
  printf("%d.total_traffic_wb \t%ld\n", core, stats->total_traffic_wb);
  printf("%d.total_traffic_wt \t%ld\n", core, stats->total_traffic_wt);

}

void print_cache_config(cache_t *cache) {
  printf(" *** Cache Configuration *** \n");
  printf("capacity   \t\t%5d B\n", cache->capacity);
  printf("block_size \t\t%5d B\n", cache->block_size);
  printf("associativity \t\t");
  if (cache->n_index_bit == 0)
    printf("fully\n");
  else
    printf("%d-way\n", cache->assoc);
  
  printf("n_set \t\t\t%d\n",cache->n_set);
  printf("n_total_cache_line \t%d\n", cache->n_total_cache_line);
  printf("tag: %d, index: %d, offset: %d\n", cache->n_tag_bit,
         cache->n_index_bit, cache->n_offset_bit);
  printf("Coherence Protocol: \t%s\n", cache->protocol == NONE ? "none" : cache->protocol == VI ? "vi" : "msi");
}

char state_to_char(enum state_t state) {
  switch(state) {
  case INVALID:
    return 'I';
  case VALID:
    return 'V';
  case SHARED:
    return 'S';
  case MODIFIED:
    return 'M';
  }
  return '-';
}


void print_insn_info(simulator_t *sim, int core, char cmd, unsigned long addr, bool hit_f) {
  printf("%d %c %lx --> {blk: %lx} %s ==> [set:%4d][way:%d](%c,%s)\n", core, cmd,
	 addr, get_cache_block_addr(sim->cache[core], addr), hit_f ? " hit" : "miss",
	 print_set, print_way, state_to_char(sim->cache[core]->lines[print_set][print_way].state),
	 sim->cache[core]->lines[print_set][print_way].dirty_f ? "dirty" : "clean");
}

