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
#include <malloc.h>

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
int powTwo(uint32 x);
Set* makeset(Set* prev, uint32 i);


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
  Cache *c;
  c=(Cache *)malloc(sizeof(Cache));
  c->set=(Set *)malloc(sizeof(Set)); 
  c->set->way=(Line *)malloc(sizeof(Line));
  c->s_access=0;
  c->s_hit=0;
  c->s_miss=0;
  c->s_evict=0;
  c->bsize=blocksize;
  c->ways=ways;
  c->sets=sets;
  c->tshift=tshift;

  c->set->setno=0;
  c->set->way->next=NULL;
  Set *prev=c->set;
  
  int i; 
  for(i=1; i<sets; i++){
    prev= makeset(prev,i);
  }
  prev->next=NULL;

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
  return c;
}

void delete_cache(Cache *c)
{
  free(c->set->way);
  free(c->set);
  free(c);
}

/*
 * line_access : update data structures to reflect access to a cache line
 * return 1 when hit, return 0 when miss
 * 1. Find set
 * 2. Check through lines for tag match
 * 3. Check the valid data
 */
int line_access(Cache *c, Line *l)
{
  Set *now=c->set;
  
  /* Find set */
  while(now->setno!=l->setno){
    now=now->next;
  }
  
  /* Search through the lines for tag match */
  Line *nowl=now->way;
  while(nowl->tag!=l->tag){
    if(nowl->next==NULL) return 0; // Miss
    nowl=nowl->next;
  }
  
  /* If the data in that line matches return hit, else miss */
  if((nowl->start<=l->start) && (nowl->end>=l->end))
    return 1;
  else return 0;
}


/*
 * line_alloc : update data structures to reflect allocation of a new block into a line
 * 1. Find set and get in there
 * 2. Find line with victim tag
 * 3. Delete it and insert newline 
 */
void line_alloc(Cache *c, Line *l, uint32 tag)
{
  Set *now=c->set;
  Set *prev;
  
  /* Find set */
  while(now->setno!=l->setno){
    now=now->next;
  }
  
  /* Find line with victim tag */
  while(now->tag!=l->tag){
    prev=now;
    now=now->next;
  }
  
  /* Replace with new line */
  prew->next=l;
  l->next=now->next;
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
  uint32 tag, set, offset, offset_max;
  uint32 addr=address;
  tag=addr/powTwo(c->tshift);
  addr-=tag*powTwo(c->tshift);
  set=addr/(c->bsize*c->ways);
  addr-=set*(c->bsize*c->ways);
  offset=addr;
  offset_max=addr+length;
  
  Line *newl=(Line *)malloc(sizeof(Line));
  newl->setno=set;
  newl->tag=tag;
  newl->start=offset;
  newl->end=offset_max;
  
  // 2. check if we have a cache hit
  int hit=0;
  hit = line_access(c, newl);
  
  // 3. on a cache miss, find a victim block and allocate according to the
  //    current policies
  if(!hit){
    uint32 vic_tag;
    vig_tag=set_find_victim(c,);
    line_alloc(c,newl,vic_tag);
  }
  // 4. update statistics (# accesses, # hits, # misses)
  c->s_access++;
  if(!hit) c->s_miss++;
  else c->s_hit++;
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

int powTwo(uint32 x){
  int i;
  int ans=1;
  for(i=1;i<=x;i++){
    ans*=2;
  }
  return ans;
}


Set* makeset(Set* prev, uint32 i){
    Set* s=(Set *)malloc(sizeof(Set));
    s->way=(Line *)malloc(sizeof(Line));
    s->way->next=NULL;
    s->setno=i;
    s->next=NULL;
    
    prev->next=s;
    prev=s;
    return s;
}
