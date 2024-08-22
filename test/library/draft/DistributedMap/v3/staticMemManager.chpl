// WARNING: this test has some race in it.  Please find it before re-enabling
// the test!
use DistributedMap;

const n = 200;
const m = 50;

var dm = new distributedMap(int, int);

var staticManager = new staticRefsManager(dm);
forall i in 0..<n with (ref staticManager) do
  manage staticManager as staticMap do // the underlying map is notified that it can't rehash right now
    staticMap[i%m] += 1;

// <- here: the map is notified that it can rehash again
//       (assuming that no other threads have checked out a staticRefsManager)

writeln("correct values: ", && reduce [i in 0..<m] dm[i] == n / m);
