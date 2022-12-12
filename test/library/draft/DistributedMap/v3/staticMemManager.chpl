// This test isn't compiling yet. `staticRefsManager` is still under development.

use DistributedMap;

var dm = new distributedMap(int, int);
writeln(dm.tables[0].numBuckets);

var staticManager = new staticRefsManager(dm);
forall i in 0..100 with (ref staticManager) do
    manage staticManager as staticMap do // the underlying map is notified that it can't rehash right now
      staticMap[i%50] += 1;

// <- here: the map is notified that it can rehash
//          (assuming that no other threads have checked out a staticRefsManager)

writeln(dm);
writeln(dm.tables[0].numBuckets);
