Performance results for aggregated atomics

Results from 48-core cascadelake nodes on horizon (Cray XC)

Bale histogram:
===

Chapel:
```
chpl --fast histo.chpl --no-optimize-forall-unordered-ops
./histo -nl 16 --N=$((10**8/24))

      OAGP:	12.444 seconds	0.129 GB/s/node
      UAGP:	3.381 seconds	0.473 GB/s/node
      AGG :	1.143 seconds	1.400 GB/s/node
```

Running a hacked version that uses non-atomic updates gets us to:

```
      HACK:	0.689 seconds	2.321 GB/s/node
```

SHMEM:
```
# Follow https://github.com/Cray/chapel-private/issues/1387:
srun --nodes=$NODES --exclusive --ntasks=$(($NODES*$CORES_PER_NODE)) --ntasks-per-node=$CORES_PER_NODE $NODE_TYPE ./build_$PLATFORM/bin/ig -c $CORES_PER_NODE -n $((10**8/24)) -M 3

       AGP:    7.225 seconds     0.221 GB/s injection bandwidth
   Exstack:    0.415 seconds     3.852 GB/s injection bandwidth
```

Table form (ordered fine grain vs aggregated):

| Config | ordered amo    | aggregated amo |
| ------ | -------------: | -------------: |
| Chapel | 0.13 GB/s/node | 1.40 GB/s/node |
| SHMEM  | 0.22 GB/s/node | 3.85 GB/s/node |


HPCC RA:
===

Chapel:
```
start_test --perflabel ml- test/release/examples/benchmarks/hpcc/ra-atomics.chpl
tail  test/perfdat/horizon/ra.atomic-{ord,uno,agg}.dat

==> test/perfdat/horizon/ra.atomic-ord.dat <==
# Date	Execution time =	Performance (GUPS) =	Validation:
02/28/22	43.1182	0.23192	SUCCESS

==> test/perfdat/horizon/ra.atomic-uno.dat <==
# Date	Execution time =	Performance (GUPS) =	Validation:
02/28/22	10.7503	0.930209	SUCCESS

==> test/perfdat/horizon/ra.atomic-agg.dat <==
# Date	Execution time =	Performance (GUPS) =	Validation:
02/28/22	3.16765	3.15691	SUCCESS
```
