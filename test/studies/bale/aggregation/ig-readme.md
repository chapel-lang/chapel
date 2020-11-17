This contains 2020/10/05 Bale indexgather performance results for Chapel vs
Reference SHMEM

---

Aggregated indexgather
===

This version of indexgather uses a copy of the aggregators we've been using in
Arkouda.

Here's the performance I see on a Cray-XC with 36-core Broadwell CPUS. I did
runs at 16 nodes and 256 nodes. For the aggregated Chapel our distributed array
indexing really starts to hurt, especially for cyclic so I did runs with block
as well and block with lazyRAD disabled. I expect us to be able to optimize
array indexing going forward, it's just not a place where local overhead
mattered before aggregation.


Summary of indexgather performance for Shmem Exstack and Chapel aggregated.
There's 3 Chapel variants:
 - using a cyclic array
 - using a block array
 - using a block array with indexing optimization on (hurts array creation time)

IndexGather (GB/s/Node):

| nodes | shmem | chpl cyc | chpl blk | chpl blk opt |
| ----- | ----- | -------- | -------- | ------------ |
|  16   | 2.89  | 1.79     | 2.79     | 3.02         |
| 256   | 1.99  | 1.39     | 1.92     | 2.04         |


---

And the full output from all variants


Reference Shmem:
---

```
export PLATFORM=cray
export CORES_PER_NODE=36
```

```
export NODES=16
aprun -n $(($NODES*CORES_PER_NODE)) -N $CORES_PER_NODE ./build_$PLATFORM/bin/ig -c $CORES_PER_NODE -n 10000000

***************************************************************
Bale Version 3.00 (OpenShmem version 1.3): 2110-10-05.08:29
Running command on 576 PEs: ./build_cray/bin/ig -c 36 -n 10000000
***************************************************************

num_requests_per_pe: 10000000
table_size_per_pe: 10000
Standard options:
----------------------------------------------------
buf_cnt (buffer size)    (-b): 1024
seed                     (-s): 122222
cores_per_node           (-c): 36
Models Mask              (-M): 15

       AGP:   23.745 seconds     0.243 GB/s injection bandwidth
   Exstack:    1.997 seconds     2.885 GB/s injection bandwidth
  Exstack2:   51.299 seconds     0.112 GB/s injection bandwidth
  Conveyor:    3.395 seconds     1.697 GB/s injection bandwidth
```


```
export NODES=256
aprun -n $(($NODES*CORES_PER_NODE)) -N $CORES_PER_NODE ./build_$PLATFORM/bin/ig -c $CORES_PER_NODE -n 10000000

***************************************************************
Bale Version 3.00 (OpenShmem version 1.3): 2110-10-05.08:27
Running command on 9216 PEs: ./build_cray/bin/ig -c 36 -n 10000000
***************************************************************

num_requests_per_pe: 10000000
table_size_per_pe: 10000
Standard options:
----------------------------------------------------
buf_cnt (buffer size)    (-b): 1024
seed                     (-s): 122222
cores_per_node           (-c): 36
Models Mask              (-M): 15

       AGP:   28.123 seconds     0.205 GB/s injection bandwidth
   Exstack:    2.895 seconds     1.989 GB/s injection bandwidth
  Exstack2:   53.279 seconds     0.108 GB/s injection bandwidth
  Conveyor:    4.299 seconds     1.340 GB/s injection bandwidth
```



Chapel:
---

```
chpl ig.chpl --fast

./ig -nl 16 --N=10000000
       AGP:     6.987 seconds   0.824 GB/s/node
       AGG:     3.217 seconds   1.791 GB/s/node

./ig -nl 256 --N=10000000
       AGP:     7.446 seconds   0.774 GB/s/node
       AGG:     4.138 seconds   1.392 GB/s/node


chpl ig.chpl --fast -suseBlockArr -o ig-block

./ig-block -nl 16 --N=10000000
       AGP:     6.823 seconds   0.844 GB/s/node
       AGG:     2.063 seconds   2.792 GB/s/node

./ig-block -nl 256 --N=10000000
       AGP:     7.290 seconds   0.790 GB/s/node
       AGG:     3.003 seconds   1.918 GB/s/node


chpl ig.chpl --fast -suseBlockArr -sdefaultDisableLazyRADOpt -o ig-block-rad

./ig-block-rad -nl 16 --N=10000000
       AGP:     6.820 seconds   0.845 GB/s/node
       AGG:     1.904 seconds   3.025 GB/s/node

./ig-block-rad -nl 256 --N=10000000
       AGP:     7.263 seconds   0.793 GB/s/node
       AGG:     2.819 seconds   2.043 GB/s/node
```
