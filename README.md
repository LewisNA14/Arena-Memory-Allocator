<h1 align="center">
  Arena Memory Allocator (C)
</h1>

<p align="center">
  <a href="#introduction">Introduction</a> •
  <a href="#features">Features</a> •
  <a href="#usage">Usage</a> •
  <a href="#api-reference">API Reference</a> •
  <a href="#build">Build</a>
</p>

---

## Introduction

A lightweight, interactive **arena (bump) allocator** written in C. It manages a fixed-size memory block through a simple command-line interface;
allocates strings and integers, inspects raw hex contents, and flushes the arena back to zero in a single call.

The project was built to explore low-level memory management concepts without relying on `malloc`/`free` for every individual allocation. 

The focus is on understanding how arena-style allocation works: reserving memory upfront, handing out chunks sequentially, and reclaiming everything at once.

---

## Features
- Fixed-size arena backed by a stack-allocated byte array
- Allocate **integers** (`long`) or **strings** with automatic type detection
- **Flush** the entire arena in a single zeroing operation
- Hex dump of all currently allocated bytes
- Used / remaining byte tracking
- Interactive CLI menu for quick manual testing

---

## Usage

Build and run the program, then interact through the menu:

```
Arena Memory Allocator Menu
Below are a number of options for allocating your memory
(1) Memory Allocation
(2) Memory Flush / Deletion
(3) Show Current Contents
(4) Show Data Usage
(q) Quit
```

| Option | Action |
|--------|--------|
| `1` | Prompts for input. Automatically detects a `long` integer or a string and allocates the appropriate amount of memory. |
| `2` | Flushes the arena — resets the offset to `0` and zeroes the entire backing buffer. |
| `3` | Prints a formatted hex dump of all currently allocated bytes. |
| `4` | Shows bytes used and bytes remaining. |
| `q` | Quits the program. |

---

## API Reference

The core arena functions are self-contained and can be used independently of the CLI.

### `ArenaInit`
```c
void ArenaInit(Arena* arena, void* memory, size_t capacity);
```
Initialises the arena over an existing memory buffer. No dynamic allocation is performed - the caller owns the backing memory.

### `ArenaAlloc`
```c
void* ArenaAlloc(Arena* arena, size_t chunk);
```
Bumps the internal offset forward by `chunk` bytes and returns a pointer to the start of the new allocation. Returns `NULL` if there is insufficient remaining capacity.

### `ArenaFlush`
```c
void ArenaFlush(Arena* arena);
```
Resets the arena offset to zero and zeroes the entire backing buffer via `memset`. All previously returned pointers should be considered invalid after this call.

### `ArenaGet`
```c
void ArenaGet(Arena* arena);
```
Prints a formatted hex dump of all bytes within the currently allocated region, 16 bytes per row.

### `ArenaRemain`
```c
void ArenaRemain(Arena* arena);
```
Prints the number of bytes used and the number of bytes still available in the arena.

---

## Build

### Requirements
- C99-compatible compiler (GCC or Clang recommended)
- Standard C library

### Configuration

The total arena size is defined at the top of `main.c`:

```c
#define ARENA_SIZE 5000
```

Adjust this value before compiling to increase or decrease available memory.

### Building from Source

#### 1. Clone the Repository
```bash
git clone https://github.com/LewisNA14/Arena-Memory-Allocator.git
cd ArenaMemoryAllocator
```

#### 2. Compile
```bash
gcc -o arena main.c
```

Or with warnings enabled (recommended):
```bash
gcc -Wall -Wextra -o arena main.c
```

#### 3. Run
```bash
./arena
```

---

## Limitations

- Arena size is fixed at compile time — it cannot grow dynamically.
- No support for individual chunk deallocation; only a full flush is available.
- No memory alignment is enforced, which may cause issues on architectures with strict alignment requirements.
