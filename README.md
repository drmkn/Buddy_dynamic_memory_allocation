# Buddy_dynamic_memory_allocation

Project - Implement replacements for the malloc,calloc, realloc and free system
calls

We have attached our current code for buddy memory allocator(balloc.c).It has a header file balloc.h. Remaining all other files are for testing purposes.

1. $gcc -g -Wall -c balloc.c (to create balloc.o).
2. $gcc -g -Wall balloc.o balloc-linked-list-traditional.c (compiling traditional linked list using our balloc) 
3. $./a.out 100 (run command for 100 inserting and then deleting nodes from linked list)
4. $gcc linked-list-traditional-v2.c (compiling normal traditional linked list code which is provided in c_lab)
5. $./a.out 100 (run command for 100 inserting and then deleting nodes from linked list)
In both of the code " balloc-linked-list-traditional.c" and " linked-list-traditional-v2.c" the seed is set to default. So we can compare execution time in both cases.

Additional testing files:
1. Testing.c : This has both types of matrix allocation (as per OS End Sem paper). balloc.c seems to be working for both. Further for second matrix allocation it is also reallocating as required.
    $gcc -g -Wall balloc.o Testing.c
   
3. Mergefiles.c : As told by you in previous meeting. This code first creates 5 txt files of integers(generated randomly). The first integer of every file is the total number of integers in that file. We store all the integers of each file in a buffer matrix using our balloc.c and then write all the integers(of every file) in a single file merge.txt. It seems to be working.
    $gcc -g -Wall balloc.o Mergefiles.c
