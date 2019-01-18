# IMSuite: IIT Madras Benchmark Suite for Simulating Distributed Algorithms

http://www.cse.iitm.ac.in/~krishna/imsuite/

IMSuite implements some of the classical distributed algorithms. These classical
algorithms cover important characteristics of distributed systems such as communication
(broadcast, unicast, or multicast), timing (synchronous, asynchronous or partially
synchronous) and failure. These algorithms have been implemented as kernel benchmarks.

IMSuite include implementation for twelve classical distributed algorithms which can be
the further subdivided into two broad cateregories- Timing and Characterization.
Based on the different form of parallelization, the kernels present in the
aforementioned heads are further subdivided under two categories: Data parallel kernels
and Recursive task parallel kernels.

The test inputs used have been taken from the input provided in the IMSuite input samples.

## Benchmark Details
- Breadth First Search
    * Bellman Ford
    * Dijkstra
- Byzantine Agreement
- Dijkstra Routing
- k-Committee
- Dominating Set
- Maximal Independent Set
- Leader Election:
    * ```LCR```
    * HS
    * DP
- Minimum Spanning Tree
- ```Vertex Coloring```

## Currently Implemented

```Vertex-Coloring``` : This algorithm colors the nodes of a tree with three colors. The algorithm starts by
initially coloring the nodes of the tree with six colors which takes O(log*n) time.
Next the algorithm uses a shift down operation (constant time) to color the tree
using three colors.

```Leader-Election: LCR```: This algorithm (known as LeLann-Chang-Roberts algorithm) finds the
leader for a set of nodes organized in a ring network, where the data flow is unidirectional.
The distinguishing feature of this algorithm is its simplicity and negligible computation.

## To learn more about the algorithms:
- Vertex Coloring :
    * https://en.wikipedia.org/wiki/Graph_coloring#Parallel_and_distributed_algorithms
    * http://www.iitg.ernet.in/gkd/aie/slide/Coloring-PSM.pdf
- Leader Election : LCR
    * https://en.wikipedia.org/wiki/Leader_election#Leader_election_in_rings
    * http://www.cs.rug.nl/~eirini/DS_slides/leader_election.pdf
