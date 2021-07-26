# Summary

1. Virtual memory abstracts physical memory to the process and presents the
available memory as a large contiguous array of bytes exclusively available to
the process.
2. Benefits of virtual memory include:
    1. A program can be larger than the available physical memory
    2. A program does not need to be in memory all the time
    3. Processes may share memory (without the need to copy)
    4. Processes may be created much more efficiently.
3. Demand paging: Pages are only fetched from swap space to memory when it's
   referenced. Pages that are never referenced are never fetched to memory.
4. Copy-on-write: A child process has the same address space as its parent,
   pages are duplicated only in the case that one process makes a modification.
5. When available memory falls to a certain threshold, a page-replacement
   algorithm selects an existing page and swaps it to the backing storage to
   free memory space. LRU replacement algorithm is usually impractical and most
   systems implement an approximation for it such as the clock algorithm.
6. Global replacement policy: Select a page from any process to swap out. Local
   replacement policy: Select a page from the process that page-faulted to swap
   out.
7. Thrashing occurs when the system spends more time paging than executing
8. Memory compression: An alternative to virtual memory; pages that are not used
   for a long time are packed together to fit into one memory page until one of
   them is referenced again.
9. Mobile systems usually do not support virtual memory since continuous writes
   to a flash backing storage makes it wear faster. Memory compression is
   alternatively used.
10. Kernel memory is usually allocated in contiguous chunks of fixed sizes, slab
    allocation works best for it.
11. TLB reach: The amount of physical memory that can be reached from a full
    TLB. TLB reach can be increased by increasing the size of pages. if the TLB
    reach is smaller than the process size, TLB misses will occur even when only
    one process is executing.
