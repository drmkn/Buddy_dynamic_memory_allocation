# Buddy Dynamic Memory Allocation Project

This project implements a custom memory allocator using the **Buddy Memory Allocation** technique for the Operating Systems Course offered at ISI Kolkata. The allocator replaces system calls like `malloc()`, `calloc()`, `realloc()`, and `free()`, and provides functions for dynamic memory management using this custom technique.

### Buddy Memory Allocation Overview
Buddy memory allocation is a memory management technique that divides memory into partitions to try to satisfy a memory request as efficiently as possible. The memory is divided into blocks, and when a block is allocated, it may be split into two smaller "buddies." These buddies can later be merged back into a single block when they are both freed.

## Files in the Project

1. **balloc.c**: Contains the implementation of the Buddy Memory Allocation system.
2. **balloc.h**: Header file for `balloc.c` providing function declarations and necessary constants.
3. **balloc-linked-list-traditional.c**: A test file where a traditional linked list uses the custom buddy allocator for node management.
4. **linked-list-traditional-v2.c**: A traditional linked list implementation using the default system allocator (for comparison with `balloc-linked-list-traditional.c`).
5. **Testing.c**: Contains matrix allocation and reallocation test cases, including two types of matrix allocations described in the OS End Sem paper.
6. **Mergefiles.c**: Merges multiple files containing random integers into a single file using a buffer matrix allocated with the custom buddy memory allocator.

## Compilation Instructions

### 1. Compile `balloc.c` into an Object File
To compile the buddy memory allocator implementation (`balloc.c`) into an object file:
```bash
gcc -g -Wall -c balloc.c
```

This generates balloc.o, the object file to be used for further testing.

### 2. Testing with Linked List (Using Buddy Allocator)
This command compiles the traditional linked list implementation, where node allocation uses the custom buddy allocator:
```bash
gcc -g -Wall balloc.o balloc-linked-list-traditional.c
```
Run the program with:
```bash
./a.out 100
```
This will perform 100 insertions and deletions of nodes in the linked list using the buddy memory allocator.

### 3. Testing with Linked List (Using Default Allocator)
To compare the performance with the default system allocator, compile and run the traditional linked list implementation (linked-list-traditional-v2.c):
```bash
gcc linked-list-traditional-v2.c
./a.out 100
```
This will perform 100 insertions and deletions of nodes using the default memory allocator.

### 4. Matrix Allocation Testing
To test matrix allocation and reallocation with the custom allocator:
```bash
gcc -g -Wall balloc.o Testing.c
./a.out
```
The matrix allocation test includes two types of matrix allocations:

Row-major order: In this method, rows of the matrix are stored contiguously in memory.
Column-major order: In this method, columns of the matrix are stored contiguously in memory.
This test will ensure that the buddy allocator can handle both type of dynamic memory allocation of arrays.



### 5. Merging Multiple Files
This program merges multiple text files containing random integers into a single file. It uses a buffer matrix allocated using the buddy allocator.
```bash
gcc -g -Wall balloc.o Mergefiles.c
./a.out
```
This creates 5 files with random integers, stores them into a buffer, and then merges them into a single output file merge.txt.

### References
- https://www.isical.ac.in/~mandar/courses.html#os
- Data Structures and Algorithms by Alfred V. Aho
- https://people.kth.se/~johanmon/ose/assignments/buddy.pdf

### Team Members
- Dhruv (https://github.com/drmkn)
- Adrish (https://github.com/adrishpaik)
- Indrajit (https://github.com/Indrajit-hub)
