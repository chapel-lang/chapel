use DistributedMap;

var dm = new distributedMap(int, int);

var staticManager = new staticRefsManager(dm);

forall i in 0..100 with (ref staticManager) do
    manage staticManager as staticMap do
      staticMap[i] += 1;
