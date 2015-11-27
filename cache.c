//------------------------------------------------------------------------------
// 4190.308                     Computer Architecture                  Fall 2015
//
// Cache Simulator Lab
//
// File: cache.c
//
// (C) 2015 Computer Systems and Platforms Laboratory, Seoul National University
//
// Changelog
// 20151119   bernhard    created
//

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include <math.h>

char RP_STR[RP_MAX+1][32] = {
  "round robin", "random", "LRU (least-recently used)",
  "MRU (most-recently used)", "LFU (least-frequently used)",
  "MFU (most-frequently used)"
};

char WP_STR[2][20] = {
  "write-allocate", "no write-allocate"
};

// Helper Functions //
int isPowerofTwo(uint32 x);
int getTwoPower(uint32 x);


Cache* create_cache(uint32 capacity, uint32 blocksize, uint32 ways,
                    uint32 rp, uint32 wp, uint32 verbosity)
{
  // 1. check cache paramenters
  //    - capacity, blocksize, and ways must be power of 2
  if(!isPowerofTwo(capacity)){
    printf("ERROR: capacity must be power of two");
    exit(0);
  }
  if(!isPowerofTwo(blocksize)){
    printf("ERROR: blocksize must be power of two");
    exit(0);
  }
  if(!isPowerofTwo(ways)){
    printf("ERROR: ways must be power of two");
    exit(0);
  }
  //    - capacity must be >= blocksize
  if(capacity<blocksize){
    printf("ERROR: capacity must not be smaller then blocksize");
    exit(0);
  }
  //    - number of ways must be <= the number of blocks
  if(ways>(capacity/blocksize)){
    printf("ERROR: ways must not be bigger then # of blocks");
    exit(0);
  }
  
  // 2-0. calculate necessary informations
  uint32 sets, tshift;
  sets = (capacity/blocksize)/ways;
  tshift = getTwoPower(blocksize)+ getTwoPower(sets);
  
  // 2. allocate cache and initialize them
  //    - use the above data structures Cache, Set, and Line
  Cache *cache;
  cache->s_access=0;
  cache->s_hit=0;
  cache->s_miss=0;
  cache->s_evict=0;
  cache->bsize=blocksize;
  cache->ways=ways;
  cache->sets=sets;
  cache->tshift=tshift;
  // TODO

  // 3. print cache configuration
  printf("Cache configuration:\n"
         "  capacity:        %6u\n"
         "  blocksize:       %6u\n"
         "  ways:            %6u\n"
         "  sets:            %6u\n"
         "  tag shift:       %6u\n"
         "  replacement:     %s\n"
         "  on write miss:   %s\n"
         "\n",
         capacity, blocksize, ways, sets, tshift, RP_STR[rp], WP_STR[wp]);

  // 4. return cache
  return cache;
}

void delete_cache(Cache *c)
{
  cache->s_access=0;
  cache->s_hit=0;
  cache->s_miss=0;
  cache->s_evict=0;
  cache->bsize=0;
  cache->ways=0;
  cache->sets=0;
  cache->tshift=0;
  (cache->)->next=NULL;
}

void line_access(Cache *c, Line *l)
{
  // update data structures to reflect access to a cache line
}

// return matching Line if hit, 0 if miss  
Line line_hitcheck(Cache *c, uint32 set, uint32 tag)
{
  while(){
    if((c->set)->line->
  }
}

void line_alloc(Cache *c, Line *l, uint32 tag)
{
  // TODO
  //
  // update data structures to reflect allocation of a new block into a line
}

uint32 set_find_victim(Cache *c, Set *s)
{
  // TODO
  //
  // for a given set, return the victim line where to place the new block
  return 0;
}

void cache_access(Cache *c, uint32 type, uint32 address, uint32 length)
{
  // simulate a cache access
  // 1. compute set & tag
  uint32 tag, set, offset;
  uint32 addr=address;
  tag=addr/pow(2,c->tshift);
  addr-=tag*pow(2,c->tshift)
  set=addr/(c->bsize*c->ways);
  addr-=set*(c->bsize*c->ways);
  offset=addr;
  
  // 2. check if we have a cache hit
  bool miss=0;
  Line *cline;
  cline= line_hitcheck(c,set, tag);
  
  // 3. on a cache miss, find a victim block and allocate according to the
  //    current policies
  if(miss){
    Line *l;
    l->tag=tag;
    l->start =offset;
    l->end = offset+(c->bsize);
    l->next = NULL;
    set_find_victim(c,);
    line_alloc(c,l,tag);
  }
  // 4. update statistics (# accesses, # hits, # misses)
  c->s_access++;
  if(miss) c->s_miss++;
  else c->hit++;
}

//----------------------Helper Function------------------------------------

int isPowerofTwo(uint32 x){
  /* While x is even and > 1 */
  while (((x & 1) == 0) && x > 1) x >>= 1;
  return (x == 1);
}

int getTwoPower(uint32 x){
  int count=0;
  /* While x is even and > 1 */
  while (((x & 1) == 0) && x > 1){
    x >>= 1;
    count++;
  }
  return count;
}
