//
// A performance test benchmarking common list operations.
//

private use HaltWrappers;
private use List;
private use Random;
private use Time;

type byte = int(8);
type testList = list(byte, false);

config const isPerformanceTest: bool = false;
config const n0: int = 50000;

const n1: int = n0 / 1000;
const seed = 314159;

proc createList(size: int) {
  var result: testList;
  for i in 1..size do result.pushBack(0:byte);
  return result;
}

proc warmup() {
  const size = if isPerformanceTest then n1 else n0;
  var lst1 = createList(size);
  var lst2 = createList(size);

  for (a, b) in zip(lst1, lst2) do a += b % 127;

  lst1.clear();
  lst2.clear();  
}

class Test {
  proc name(): string { pureVirtualMethodHalt(); }
  proc setup() {}
  proc test() { pureVirtualMethodHalt(); }

  proc run(): real {
    var tmr = new stopwatch();

    setup();

    tmr.start();
    test();
    tmr.stop();

    return tmr.elapsed();
  }

  proc output() {
    // Run tests even if we aren't `-performance`.
    const time = this.run();

    const msg = if isPerformanceTest
      then this.name() + " " + time:string
      else this.name();
    writeln(msg);
  }
}
  
class AppendFromEmpty: Test {
  var _lst: testList;

  override proc name() do return "Append";
  override proc setup() { _lst.clear(); }

  override proc test() {
    for i in 1..n0 do _lst.pushBack((i % 127):byte);
  }
}

class InsertFront: Test {
  // Use a smaller value for N because InsertFront is a O(n**2) operation.
  var _lst: testList;
 
  override proc name() do return "InsertFront";
  override proc setup() { _lst = createList(1); }
  override proc test() {
    while _lst.size < n1 do _lst.insert(0, (_lst.size & 127):byte);
  }
}

class PopFromBack: Test {
  var _lst: testList;

  override proc name() do return "PopBack";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    while !_lst.isEmpty() do _lst.popBack();
  }
}

class PopFromFront: Test {
  var _lst: testList;

  override proc name() do return "PopFront";
  // Use a smaller value for N because PopFront is O(n**2).
  override proc setup() { _lst = createList(n1); }
  override proc test() {
    while !_lst.isEmpty() do _lst.pop(0);
  }
}

class IterSerial: Test {
  var _lst: testList;

  override proc name() do return "IterSerial";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    for x in _lst do x += 1;
  }
}

class IterParallel: Test {
  var _lst: testList;

  override proc name() do return "IterParallel";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    forall x in _lst do x += 1;
  }
}
  
class RandomAccess1: Test {
  var _lst: testList;
  var _rnd: [0..#n0] int;

  override proc name() do return "RandomAccess1";
  override proc setup() {
    _lst = createList(n0);
    // Set up a trace of random indices.
    fillRandom(_rnd, seed);
    _rnd = mod(_rnd, n0);
  }
  override proc test() {
    for r in _rnd do _lst[r] &= 0xFF:byte;
  }
}

class Clear: Test {
  var _lst: testList;

  override proc name() do return "Clear";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    _lst.clear();
  }
}

proc main() {
  var tests: list(owned Test);

  tests.pushBack(new AppendFromEmpty());
  tests.pushBack(new InsertFront());
  tests.pushBack(new PopFromBack());
  tests.pushBack(new PopFromFront());
  tests.pushBack(new IterSerial());
  tests.pushBack(new IterParallel());
  tests.pushBack(new RandomAccess1());
  tests.pushBack(new Clear());

  warmup();

  for tst in tests do
    tst.output();
}
