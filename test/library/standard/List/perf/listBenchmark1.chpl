//
// A performance test benchmarking the following common list operations:
//
//    - Insertions of various sizes
//    - Removal of various sizes
//    - 
//    - Sequential-access indexing
//    - Random-access indexing
//

private use HaltWrappers;
private use List;
private use Random;
private use Search;
private use Sort;
private use Time;

type byte = int(8);
type testList = list(byte, false);

config const trials = 5;
config const n: int = 1024 * 32;
const seed = 314159;
const stableList = createList(n);

proc createList(size: int) {
  var result: testList;
  for i in 1..size do result.append((i % 127):int(8));
  return result;
}

proc setup() {
  extern proc srand(seed: int);
  srand(seed);
}

proc randInt(): int {
  extern proc rand(): int;
  return rand();
}

proc generateNoise() {
  var lst1 = createList(n);
  var lst2 = createList(n);

  for (a, b) in zip(lst1, lst2) do a += b % 127;

  lst1.clear();
  lst2.clear();  
}

class Test {
  proc name(): string { pureVirtualMethodHalt(); }
  proc setup() {}
  proc test() { pureVirtualMethodHalt(); }

  proc run(): real {
    var tmr = new Timer();
    var avg = 0.0;

    for i in 1..trials {
      setup();
      tmr.start();
      test();
      tmr.stop();
      avg += tmr.elapsed();
    }

    avg /= trials;

    return avg;
  }

  proc output() {
    writeln(this.name(), " ", this.run());
  }
}
  
class AppendFromEmpty: Test {
  var _lst: list(byte);

  override proc name() return "AppendFromEmpty";
  override proc setup() { _lst.clear(); }

  override proc test() {
    assert(_lst.isEmpty());
    for i in 1..n do _lst.append((i % 127):byte);
  }
}

class InsertFront: Test {
  const _limit = (n / 16);
  var _lst: list(byte);
  
  override proc name() return "InsertFront";
  override proc setup() {
    _lst.clear();
    for i in 1.._limit do _lst.append((i % 127):byte);
  }

  override proc test() {
    assert(_lst.size == _limit);
    for i in 1.._limit do _lst.insert(1, (i & 127):byte); 
  }
}

class PopFromBack: Test {
  override proc name() return "PopFromBack";
  override proc test() {

  }
}

class PopFromFront: Test {
  override proc name() return "PopFromFront";
  override proc test() {

  }
}

class IterSerial: Test {
  override proc name() return "IterSerial";
  override proc test() {

  }
}

class IterParallel: Test {
  override proc name() return "IterParallel";
  override proc test() {

  }
}
  
class RandomAccess1: Test {
  override proc name() return "RandomAccess1";
  override proc test() {

  }
}

class Clear: Test {
  override proc name() return "Clear";
  override proc test() {

  }
}

class Deinit: Test {
  override proc name() return "Deinit"; 
  override proc test() {

  }
}

proc main() {
  setup();

  var tests: list(owned Test);

  tests.append(new AppendFromEmpty());
  tests.append(new InsertFront());
  tests.append(new PopFromBack());
  tests.append(new PopFromFront());
  tests.append(new IterSerial());
  tests.append(new IterParallel());
  tests.append(new RandomAccess1());
  tests.append(new Clear());
  tests.append(new Deinit());

  generateNoise();

  for tst in tests do
    tst.output();
}

