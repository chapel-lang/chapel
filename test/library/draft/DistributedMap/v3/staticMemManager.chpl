use DistributedMap;

var dm = new distributedMap(int, int);
var staticManager = new staticRefsManager(dm);

forall i in 0..100 with (ref staticManager) do
    manage staticManager as staticMap do // the underlying map is notified that it can't rehash right now
      staticMap[i] += 1;

// <- here: the map is notified that it can rehash
//          (assuming that no other threads have checked out a staticRefsManager)
