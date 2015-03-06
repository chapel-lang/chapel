use Random, Time;

config const n: int = 30000;
config const randSeed: int = 11;

class Runner {
  proc run(A) { halt("virtual function run called"); }
  proc runList(ref L: list(int)) { halt("virtual function runList called");}
}

class PushBack: Runner {
  const n: int;
  proc run(A: [] int) {
    for i in 1..n {
      A.push_back(i);
    }
  }
}

class SumElements: Runner {
  const n: int;
  proc run(A: [] int) {
    var sum = 0;
    for a in A {
      sum += a;
    }
    assert(sum == n*(n+1)/2);
  }
}

class PushFront: Runner {
  const n: int;
  proc run(A: [] int) {
    for i in 1..n {
      A.push_front(i);
    }
  }
}

class PopBack: Runner {
  proc run(A: [] int) {
    while !A.isEmpty() {
      A.pop_back();
    }
  }
}

class PopFront: Runner {
  proc run(A: [] int) {
    while !A.isEmpty() {
      A.pop_front();
    }
  }
}

// time inserting into a random index in the array
class InsertRandom: Runner {
  const n: int;
  proc run(A: [] int) {
    extern proc srand(int);
    extern proc rand(): int;
    srand(randSeed);
    for i in 1..n {
      A.insert(i, (rand() % i) + 1);
    }
  }
}

// generate random numbers and put them in order in the array
class InsertSorted: Runner {
  const n: int;
  const linear: bool = false;
  proc run(A: [] int) {
    extern proc srand(int);
    extern proc rand(): int;
    srand(randSeed);
    for i in 1..n {
      const val = rand();
      const (_, loc) = if linear then LinearSearch(A, val) else BinarySearch(A, val);
      A.insert(loc, val);
    }
  }
}

// try a few similar operation on a list to compare
class ListAppend: Runner {
  const n: int;
  proc runList(ref L: list(int)) {
    for i in 1..n {
      L.append(i);
    }
  }
}

class SumReduceList: Runner {
  const n: int;
  proc runList(ref L: list(int)) {
    var sum = 0;
    for val in L {
      sum += val;
    }
    assert(sum == n*(n+1)/2);
  }
}

class ListDestroy: Runner {
  proc runList(ref L: list(int)) {
    L.destroy();
  }
}


proc timeRun(r: Runner, A) {
  var t = new Timer();
  t.start();
  r.run(A);
  t.stop();
  return t.elapsed();
}

proc timeRunList(r: Runner, ref L: list(int)) {
  var t = new Timer();
  t.start();
  r.runList(L);
  t.stop();
  return t.elapsed();
}

proc isSorted(A, n) {
  var prev = A[1];
  for i in 1..n {
    if prev > A[i] then return false;
  }
  return true;
}

config const printTimes = false;

proc output(name: string, time: real) {
  if printTimes then
    writeln(name, " ", time);
  else
    writeln(name);
}

proc main {
  var A: [1..0] int;
  var r: Runner;

  r = new PushBack(n); output("PushBack", timeRun(r, A)); delete r;
  r = new SumElements(n); output("SumElements", timeRun(r, A)); delete r;
  r = new PopBack(); output("PopBack", timeRun(r, A)); delete r;

  r = new PushFront(n); output("PushFront", timeRun(r, A)); delete r;
  r = new PopFront(); output("PopFront", timeRun(r, A)); delete r;

  r = new InsertRandom(n); output("InsertR", timeRun(r, A)); delete r;

  r = new PopBack(); r.run(A); delete r; // clean up
  r = new InsertSorted(n); output("InsertSB", timeRun(r, A)); delete r;

  assert(isSorted(A,n));

  r = new PopBack(); r.run(A); delete r; // clean up
  r = new InsertSorted(n, false); output("InsertSL", timeRun(r,A)); delete r;
  assert(isSorted(A,n));

  var l = new list(int);
  r = new ListAppend(n); output("ListAppend", timeRunList(r,l)); delete r;
  assert(l.length == n);
  r = new SumReduceList(n); output("ListReduce", timeRunList(r, l)); delete r;
  r = new ListDestroy(); output("ListDest", timeRunList(r,l)); delete r;
}
