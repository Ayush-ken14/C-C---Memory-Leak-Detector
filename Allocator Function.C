

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
