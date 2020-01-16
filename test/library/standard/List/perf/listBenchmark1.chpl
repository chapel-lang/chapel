//
// A performance test benchmarking the following common list operations:
//
//    - Insertions of various sizes
//    - Removal of various sizes
//    - 
//    - Sequential-access indexing
//    - Random-access indexing
//

private use List;
private use Random;
private use Search;
private use Sort;
private use Time;

type byte = int(8);
type testList = list(byte, false);

config const trials = 6;
config const n: int = 1024 * 1024 * 16;
const seed = 314159;

proc createList(size: int) {
  var result: testList;
  for i in 1..size do result.append(i);
  return result;
}

proc randSetup() {
  // TODO: Give the random number generator our seed.  

}

proc randInt(): int {

}

proc generateNoise() {
  var lst1 = createList(n);
  var lst2 = createList(n);

  for (a, b) in zip(lst1, lst2) do a += b % 128;

  lst1.clear();
  lst2.clear();  
}

//
// Generate some noise to warm up before the test.
//
generateNoise();

proc output(name: string, time: real) {
  writeln(name, " ", time);
}

var tmr: timer;
const stableList = createList(n);
var avg = 0.0;

proc testCreate(size: int): real {

}

avg = 0.0;
for i in 1..trials do avg += testCreate(n);
avg /= trials;
output("Create", avg);

proc testAppendFromEmpty(size: int): real {

}

avg = 0.0;
for i in 1..trials do avg += testAppendFromEmpty(n);
avg /= trials;
output("AppendFromEmpty", avg);

proc testInsertFront(size: int): real {

}

avg = 0.0;
for i in trials do avg += testInsertFront(n, trace);
avg /= trials;
output("InsertFront", avg);

proc testPopFromBack(size: int): real {

}

avg = 0.0;
for i in trials do avg += testPopFromBack(n, trace);
avg /= trials;
output("PopFromBack", avg);

proc testPopFromFront(size: int): real {

}

avg = 0.0;
for i in trials do avg += testPopFromFront(n, trace);
avg /= trials;
output("PopFromFront", avg);

proc testIterSerial(size: int): real {

}

avg = 0.0;
for i in trials do avg += testIterSerial(n, trace);
avg /= trials;
output("IterSerial", avg);

proc testIterParallel(size: int): real {

}

avg = 0.0;
for i in trials do avg += testIterParallel(n, trace);
avg /= trials;
output("IterParallel", avg);

proc testRandomAccess1(size: int): real {

}

avg = 0.0;
for i in trials do avg += testRandomAccess1(n, trace);
avg /= trials;
output("RandomAccess1", avg);

proc testClear(size: int): real {

}

avg = 0.0;
for i in trials do avg += testClear(n, trace);
avg /= trials;
output("Clear", avg);

proc testDeinit(size: int): real {

}

avg = 0.0;
for i in trials do avg += testDeinit(n, trace);
avg /= trials;
output("Deinit", avg);

