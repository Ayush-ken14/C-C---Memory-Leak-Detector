#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXM_ALLOCATION_MEMORY 100
#define WARN(msg, line) (printf("Warning %d: %s\n", line, msg))

/* 
Data Structure to keep track of memory allocations
*/

typedef struct {
    size_t address;
    size_t size;
    uint32_t line;
} Mem;

struct {
    Mem mem[MAXM_ALLOCATION_MEMORY];
    size_t total_allocated_size;
    size_t total_free_size;
} data;
