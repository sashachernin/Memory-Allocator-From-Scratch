# Memory-Allocator-From-Scratch
Memory allocator from scratch, written in C for the Linux environment. Includes malloc, realloc and free.

## Compiling
run 'make' command in terminal, and then run the "mm_test" file.

## Understanding Test
In case of allocation attempt, a writing and reading test will be done. If reading test is successful, 'done' will be written next to the allocation attempt, and 'failed' otherwise. If writing test is successful, 'ok' will be written next to the allocation attempt, and 'fail' otherwise. In case of Reallocation, a reading test will be done to check if the content is preserved, if successful, 'ok' will be written next to the reallocation attempt, 'fail' otherwise.

For example, if we want to allocate 3 times, this will be the output:

0 done ok

1 done ok

2 done ok


That means that in each allocation, both writing and reading were successful. 'done' - successful reading, 'ok' - successful writing.
