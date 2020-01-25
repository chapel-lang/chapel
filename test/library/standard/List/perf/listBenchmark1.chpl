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

param const isPrintTimes = false;
config const n0: int = 32 * 1024;

const n1: int = n0 / 16;
const seed = 314159;
const stableList = createList(n);

proc createList(size: int) {
  var result: testList;
  for i in 1..size do result.append(0:int(8));
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

    setup();
    tmr.start();
    test();
    tmr.stop();
    result = tmr.elapsed();
    tmr.clear();

    return result;
  }

  proc output() {
    const msg = if isPrintTimes
      then this.name() + " " + this.run()
      else this.name();
    writeln(msg)
  }
}
  
class AppendFromEmpty: Test {
  var _lst: testList;

  override proc name() return "AppendFromEmpty";
  override proc setup() { _lst.clear(); }

  override proc test() {
    assert(_lst.isEmpty());
    for i in 1..n0 do _lst.append((i % 127):byte);
  }
}

class InsertFront: Test {
  // Use a smaller value of N because InsertFront is a O(n**2) operation.
  const _hi = n1 / 2;
  var _lst: testList;
 
  override proc name() return "InsertFront";
  override proc setup() { _lst = createList(_hi); }

  override proc test() {
    assert(_lst.size == _hi);
    for i in 1.._hi do _lst.insert(1, (i & 127):byte); 
  }
}

class PopFromBack: Test {
  var _lst: testList;

  override proc name() return "PopFromBack";
  override proc setup() { _lst = createList(n0); }

  override proc test() {
    assert(_lst.size == n0);
    while !_lst.isEmpty() do _lst.pop();
  }
}

class PopFromFront: Test {
  override proc name() return "PopFromFront";
  // Use a smaller value of N because PopFromFront is O(n**2).
  override proc setup() { _lst = createList(n1); }
  override proc test() {
    assert(_lst.size == n1);
    while !_lst.isEmpty() do _lst.pop(1);
  }
}

class IterSerial: Test {
  var _lst: testList;
  override proc name() return "IterSerial";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    assert(_lst.size == n0);
    for x in _lst do x += 1;
  }
}

class IterParallel: Test {
  var _lst: testList;
  override proc name() return "IterParallel";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    assert(_lst.size == n0);
    forall x in _lst do x += 1;
  }
}
  
class RandomAccess1: Test {
  var _lst: testList;
  override proc name() return "RandomAccess1";
  override setup() { _lst = createList(n0); } 
  override proc test() {
    for x in _lst {
      const idx = abs(rand()) % _lst.size + 1;
      const val = _lst[idx];
    }
  }
}

class Clear: Test {
  var _lst: testList;
  override proc name() return "Clear";
  override proc setup() { _lst = createList(n0); }
  override proc test() {
    assert(_lst.size == n0);
    _lst.clear();
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

  generateNoise();

  for tst in tests do
    tst.output();
}

