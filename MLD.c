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

/**
 * Find a memory by its address
 * 
 * @return: Pointer to memory
*/
Mem *find_by_address(size_t address) {
    for (uint32_t i=0; i<MAXM_ALLOCATION_MEMORY; i++) {
        if (data.mem[i].address == address)
            return &data.mem[i]; // as soon as find return
    }

    // otherwise return null
    return NULL;
}



// insert memory allocated with size

void insert(size_t address, size_t size, uint32_t line) {
    // check for null
    if (address == 0) {
        WARN("Memory allocation failed", line);
        return;
    }

    Mem *mem = find_by_address(0);
    // if the return value is null we need to increase the MAXM_ALLOCATION_MEMORY value
    if (mem == NULL) {
        WARN("Max allocations reached", line);
        return;
    }

    // save all the allocation info
    mem->address = address;
    mem->size = size;
    mem->line = line;
    data.total_allocated_size += size;
}

/**
 * Remove the memory allocation
*/

int erase(size_t address, uint32_t line) {
    if (address == 0) {
        WARN("Tried to free a null ptr", line);
        return -1;
    }

    Mem *mem = find_by_address(address);
    // if the address is not found we assume it is already deleted
    if (mem == NULL) {
        WARN("Double free detected", line);
        return -1;
    }

    // set address to null and update info
    mem->address = 0;
    data.total_free_size += mem->size;
    return 0;
}

void CHECK_LEAK() {
    printf("\nLeak Summary\n");
    printf("Total Memory allocated %lu bytes\n", data.total_allocated_size);
    printf("Total Memory freed     %lu bytes\n", data.total_free_size);
    printf("Memory Leaked          %lu bytes\n\n", 
        data.total_allocated_size - data.total_free_size);

    if (data.total_free_size != data.total_allocated_size) {
        printf("Detailed Report\n");
        for (int i=0; i<MAXM_ALLOCATION_MEMORY; i++) {
            if (data.mem[i].address != 0) {
                printf("Memory leak at line %d: (%lu bytes)\n", 
                    data.mem[i].line,
                    data.mem[i].size);
            }
        }
    }
}

// Override allocation functions
void *_malloc(size_t size, uint32_t line) {
    void *ptr = malloc(size);

    // insert to memory data
    insert((size_t)ptr, size, line);

    return ptr;
}

void _free(void *ptr, uint32_t line) {
    // erase memory data
    if (erase((size_t)ptr, line) == 0)
        free(ptr);
}

// redefine allocator functions
#define malloc(size) _malloc(size, __LINE__)
#define free(ptr) _free(ptr, __LINE__)

int main() {

    int *n = malloc(2*sizeof(double));
    free(n);

    int *n1 = malloc(sizeof(int));
    free(n1);

    int *n2 =  malloc(4*sizeof(char));
    // free(n2);

    int *n3 = malloc(sizeof(int));
    free(n3);

    int *n4 = malloc(sizeof(int));
    free(n4);

    CHECK_LEAK();
    return 0;
}