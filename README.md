# Cache Simulator

#### Credit
This project idea and template code, as seen in the repos first commit, was created by [Justin Goins.](http://eecs.oregonstate.edu/people/Goins-Justin)
***

This is a Cache Simulator that is configurable. The simulator is able to handle caches with varying capacities, block sizes, levels of associativity, replacement policies, and write policies. The simulator operates on trace files that indicate memory access properties. All input files to the simulator follows a specific structure.

After execution is finished, the simulator generates one or more output file(s) containing information on the number of cache misses, hits, and miss evictions (i.e. the number of block replacements). The file also provides the total number of pseudo-clock cycles used during the simulation.

A couple of significant assumptions are used for the sake of simplicity.

* The simulator does not simulate the actual data contents. It simply pretends that it copied data from main memory and keeps track of the hypothetical time that would have elapsed.
* When a block is modified in the cache or in main memory, the simulator always assume that the entire block is read or written. This means that the simulator doesn't deal with the situation where only part of a block needs to be updated in main memory.
* Assume that all memory accesses occur only within a single block at a time. In other words, the simulator doesn’t worry about the effects of a memory access overlapping two blocks, it just pretends the second block was not affected.

### Cache Simulator Input
```
# example invocation of cache simulator
cache_sim ./cache_config ./tracefile1
Output file written to ./tracefile1.out
```
***
__The first command line argument__ will be the path to the configuration file. This file contains information about the cache design. The file will contain only numeric values, each of which is on a separate line.

Example contents of a configuration file:
```
8 <-- number of sets in cache (will be a non-negative power of 2)
16 <-- block size in bytes (will be a non-negative power of 2)
3 <-- level of associativity (number of blocks per set)
1 <-- replacement policy (will be 0 for random replacement, 1 for LRU)
1 <-- write policy (will be 0 for write-through, 1 for write-back)
13 <-- number of cycles required to write or read a block from the cache
230 <-- number of cycles required to write or read a block from main memory
0 <-- cache coherence protocol (0 for simple implementation)
```

Here is another example configuration file specifying a direct-mapped cache with 64 entries, a 32 byte block size, associativity level of 1 (direct-mapped), least recently used (LRU) replacement policy, write-through operation, 26 cycles to read or write data to the cache, and 1402 cycles to read or write data to the main memory.
```
64
32
1
1
0
26
1402
0
```
***
__The second command line argument__ indicates the path to a trace file. This trace file will follow the format used by Valgrind (a memory debugging tool). The file consists of comments and memory access information. Any line beginning with the ‘=’ character should be treated as a comment and ignored.

```
==This is a comment and can safely be ignored.
==An example snippet of a Valgrind trace file
I  04010173,3
I  04010176,6
 S 04222cac,1
I  0401017c,7
 L 04222caf,8
I  04010186,6
I  040101fd,7
 L 1ffefffd78,8
 M 04222ca8,4
I  04010204,4
```
Memory access entries will use the following format in the trace file:
`[space]operation address,size`

* Lines beginning with an ‘I’ character represent an instruction load. For this assignment, you can ignore instruction read requests and assume that they are handled by a separate instruction cache.
* Lines with a space followed by an ‘S’ indicate a data store operation. This means that data needs to be written from the CPU into the cache or main memory (possibly both) depending on the write policy.
* Lines with a space followed by an ‘L’ indicate a data load operation. Data is loaded from the cache into the CPU.
* Lines with a space followed by an ‘M’ indicate a data modify operation (which implies a special case of a data load, followed immediately by a data store).

### Cache Simulator output
The format of each output line is as follows (and can list up to 3 cache impacts):
`operation address,size <number_of_cycles> <cache_impact1> <cache_impact2>`

The next-to-last line of the output file indicates the number of hits, misses, and evictions. The final line indicates the total number of simulated cycles that were necessary to simulate the trace file.

In order to illustrate the output file format let’s look at an example. Suppose we are simulating a direct-mapped cache operating in write-through mode. Note that the replacement policy does not have any effect on the operation of a direct-mapped cache. Assume that the configuration file told us that it takes 13 cycles to access the cache and 230 cycles to access main memory. Keep in mind that a hit during a __load__ operation only accesses the cache while a miss must access __both the cache and the main memory__.

In this example the cache is operating in write-through mode so a standalone (S)tore operation takes 243 cycles, __even if it is a hit__, because we always write the block into both the cache and into main memory. If this particular cache was operating in write-back mode, a (S)tore operation would take only 13 cycles if it was a hit (since the block would not be written into main memory until it was evicted).

The exact details of whether an access is a hit or a miss is entirely dependent on the specific cache design (block size, level of associativity, number of sets, etc). Your program will implement the code to see if each access is a hit, miss, eviction, or some combination.

```
==For this example we assume that addresses 04222cac, 04222caf, and 04222ca8 are all in the same block at index 2
==Assume that addresses 047ef249 and 047ef24d share a block that also falls at index 2.
==Since the cache is direct-mapped, only one of those blocks can be in the cache at a time.
==Fortunately, address 1ffefffd78 happens to fall in a different block index (in our hypothetical example).
==The output file for our hypothetical example:
S 04222cac,1 243 miss
L 04222caf,8 13 hit
M 1ffefffd78,8 486 miss hit <-- notice that this (M)odify has a miss for the load and a hit for the store
M 04222ca8,4 256 hit hit <-- notice that this (M)odify has two hits (one for the load, one for the store)
S 047ef249,4 243 miss eviction <-- 243 cycles for miss, no eviction penalty for write-through cache
L 04222caf,8 243 miss eviction
M 047ef24d,2 486 miss eviction hit <-- notice that this (M)odify initially misses, evicts the block, and then hits
L 1ffefffd78,8 13 hit
M 047ef249,4 256 hit hit
Hits:8 Misses:5 Evictions:3
Cycles: 2239 <-- total sum of simulated cycles (from above)
```
