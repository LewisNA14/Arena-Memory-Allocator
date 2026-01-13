/*
    FILE: main.c

    DESCRIPTION:
    Includes the creation, destruction and management of Arena Memory allocators.

*/

// Includes
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// Defines
#define ARENA_SIZE 5000

// Struct Decleration
typedef struct{
    uint8_t* base;          // Base pointer for memory address
    size_t offset;          // Tracker of bytes used in array
    size_t capacity;        // Number of bytes free
    int no_chunks;          
} Arena;

// Initialises the Arena values
void ArenaInit(Arena* arena, void* memory, size_t capacity)
{
    arena->base = (uint8_t*)memory;
    arena->capacity = capacity;
    arena->offset = 0;
    arena->no_chunks = 0;
}

// Clears the Memory Allocation for the Arena
void ArenaFlush(Arena* arena)
{
    arena->offset = 0;                          // Reset allocation
    arena->no_chunks = 0;
    memset(arena->base, 0, arena->capacity);    // Clear out the Memory block
}


// Allocates chunks in the Arena to be reserved
void* ArenaAlloc(Arena* arena, size_t chunk)
{
    if (arena->offset + chunk > arena->capacity)
        return NULL;

    // Assign Pointer
    void* ptr = arena->base + arena->offset; // start of chunk

    // Increment offset and number of chunks
    arena->offset += chunk;
    arena->no_chunks++;

    return ptr;
}

// Gets the contents of the arena and prints it out
void* ArenaGet(Arena* arena)
{
    printf("Arena contents (%ld bytes):\n", arena->offset);
    for (size_t i = 0; i < arena->offset; i++)
    {
        // Prints the contents in a Hexadecimal format
        printf("%02X ", arena->base[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

int main()
{
    // Defines
    Arena arena;
    uint8_t Memory_Block[ARENA_SIZE];
    size_t Chunk_size;

    // Initialising the Arena Memory 
    ArenaInit(&arena, Memory_Block, ARENA_SIZE); // initialize first
    ArenaFlush(&arena);                          // Optional reset

    // Diags
    printf("Initialising the Memory Allocator \n");
    printf("offset   = %ld\n", arena.offset);
    printf("base     = %p\n", (void*)arena.base);
    printf("capacity = %ld\n", arena.capacity);
    printf("Chunks   = %i\n", arena.no_chunks);

    // Populating the Arena Memory with an instance
    uint8_t value1 = 14;
    size_t* p1 = ArenaAlloc(&arena, sizeof(value1));

    // Check no NULL was passed
    if(!p1)
    {
        printf("The Arena has reached max capacity, it's time to flush \n");
        ArenaFlush(&arena);
    }

    *p1 = value1;

    // Diagnostic Messages
    printf("Populating the Arena Memory Allocator \n");
    printf("offset   = %ld\n", arena.offset);
    printf("Chunks   = %i\n", arena.no_chunks);
    
    // Retrieving the Memory stored so far
    ArenaGet(&arena); 

    uint8_t value2 = 168;
    uint8_t* p2 = ArenaAlloc(&arena, sizeof(uint8_t));
    
    if(!p1)
    {
        printf("The Arena has reached max capacity, it's time to flush \n");
        ArenaFlush(&arena);
    }
    
    *p2 = value2;

    printf("offset   = %ld\n", arena.offset);
    printf("Chunks   = %i\n", arena.no_chunks);
    ArenaGet(&arena); 

    return 0;
}