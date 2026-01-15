/*
    FILE: main.c

    DESCRIPTION:
    Includes the creation, destruction and management of Arena Memory allocators.

*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
void ArenaGet(Arena* arena)
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

void ArenaRemain(Arena* arena)
{
    // This function shows the used / remaining bytes
    int remaining = arena->capacity - arena->offset;

    printf("Used : %ld Bytes \n", arena->offset);

    if (remaining)
    {
        printf("Remaining : %i \n", remaining);
    }
    else
    {
        printf("Remaining : 0 \n");
    }
}

int main()
{
    // Defines
    Arena arena;
    int input;
    char Alloc;

    // Memory Defines
    uint8_t Memory_Block[ARENA_SIZE];
    size_t Chunk_size;
    int remaining;
    

    // Alloc Parameters

        // Pointers
    void* p1;
    char* endptr;

        // Input Parameters
    char buffer[64];
    long num;

    // Initialising the Arena Memory 
    ArenaInit(&arena, Memory_Block, ARENA_SIZE);

    while(1)
    {
        printf("Arena Memory Allocator Menu \n"
        "Below are a number of options for allocating your memory \n"
        "(1) Memory Allocation \n"
        "(2) Memory Flush / Deletion \n"
        "(3) Show Current Contents \n"
        "(4) Show Data Usage \n"
        "(q) Quit\n"
        );

        printf("Enter your choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin))
            continue;

        // remove newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

        // Check if user quit
        if (buffer[0] == 'q') break;

        input = buffer[0] - '0'; // convert char '1' → int 1

        printf("\n----------------------------\n");
        switch(input)
        {

            case 1: // Arena Allocation
                printf("Enter the data you would like to store to the Arena Memory: \n");
                fgets(buffer, sizeof(buffer), stdin);
                
                // Attempt to convert to long integer
                num = strtol(buffer, &endptr, 10);

                // Input is a string
                if (endptr == buffer) 
                {
                    printf("Input is a string\n");
                    
                    // Get the length of the buffer including endpoint / newline
                    size_t buff_len = strlen(buffer) + 1;

                    char *s = ArenaAlloc(&arena, buff_len);
                    memcpy(s, buffer, buff_len);
                }
                // Unknown data type 
                else if (*endptr != '\n' && *endptr != '\0') 
                {
                    printf("Extra characters");
                }
                // Input is a integer
                else 
                {
                    printf("Valid integer\n");
                    long *i = ArenaAlloc(&arena, sizeof(long));
                    *i = num;
                }
                break;

            case 2: // Arena Flush
                ArenaFlush(&arena);
                printf("Memory has been flushed");
                break;

            case 3: // Arena Get / Show Contents
                ArenaGet(&arena);
                break;

            case 4: // Arena Remaining / Used
                ArenaRemain(&arena);
                break;

            default:
                printf("Invalid option");
                break;
        }
        // Formatting
        printf("\n----------------------------\n");

        printf("Press Enter to return to the menu...");
        
        // Scans for keyboard input
        fgets(buffer, sizeof(buffer), stdin); // wait for enter
    }
}
