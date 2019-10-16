use Random, Time, Search, Sort, LinkedLists;

config const n: int = 30000;
const linearN: int = n/1000; // problem size for slow "linear" opeations
config const randSeed: int = 11;

class Runner {
  proc run(A) { halt("virtual function run called"); }
  proc runList(ref L: LinkedList(int)) { halt("virtual function runList called");}
}

class PushBack: Runner {
  const n: int;
  override proc run(A: [] int) {
    for i in 1..n {
      A.push_back(i);
    }
  }
}

class SumElements: Runner {
  const n: int;
  override proc run(A: [] int) {
    var sum = 0;
    for a in A {
      sum += a;
    }
    assert(sum == n*(n+1)/2);
  }
}

class PushFront: Runner {
  const n: int;
  override proc run(A: [] int) {
    for i in 1..n {
      A.push_front(i);
    }
  }
}

class PopBack: Runner {
  override proc run(A: [] int) {
    while !A.isEmpty() {
      A.pop_back();
    }
  }
}

class PopFront: Runner {
  override proc run(A: [] int) {
    while !A.isEmpty() {
      A.pop_front();
    }
  }
}

// time inserting into a random index in the array
class InsertRandom: Runner {
  const n: int;
  override proc run(A: [] int) {
    extern proc srand(seed: int);
    extern proc rand(): int;
    srand(randSeed);
    for i in 1..linearN {
      A.insert((rand() % i) + 1, i);
    }
  }
}

// generate random numbers and put them in order in the array
class InsertSorted: Runner {
  const n: int;
  override proc run(A: [] int) {
    extern proc srand(seed: int);
    extern proc rand(): int;
    srand(randSeed);
    for i in 1..linearN {
      const val = rand();
      const (_, loc) = binarySearch(A, val);
      A.insert(loc, val);
    }
  }
}

class Remove: Runner {
  const n: int;
  const front: bool;
  override proc run(A: [] int) {
    if front {
      for i in 1..linearN {
        A.remove(1);
      }
    } else {
      var numElements = A.numElements;
      for i in 1..n {
        A.remove(numElements);
        numElements -= 1;
      }
    }
  }
}

// try a few similar operation on a list to compare
class ListAppend: Runner {
  const n: int;
  override proc runList(ref L: LinkedList(int)) {
    for i in 1..n {
      L.append(i);
    }
  }
}

class SumReduceList: Runner {
  const n: int;
  override proc runList(ref L: LinkedList(int)) {
    var sum = 0;
    for val in L {
      sum += val;
    }
    assert(sum == n*(n+1)/2);
  }
}

class ListDestroy: Runner {
  override proc runList(ref L: LinkedList(int)) {
    L.destroy();
  }
}


proc timeRun(r: borrowed Runner, A) {
  var t = new Timer();
  t.start();
  r.run(A);
  t.stop();
  return t.elapsed();
}

proc timeRunList(r: borrowed Runner, ref L: LinkedList(int)) {
  var t = new Timer();
  t.start();
  r.runList(L);
  t.stop();
  return t.elapsed();
}

proc isSorted(A: [] int, n:int): bool {
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
  var r = new Runner(); // dummy initialization

  r = new owned PushBack(n); output("PushBack", timeRun(r, A));
  r = new owned SumElements(n); output("SumElements", timeRun(r, A));
  r = new owned PopBack(); output("PopBack", timeRun(r, A));

  r = new owned PushFront(n); output("PushFront", timeRun(r, A));
  r = new owned PopFront(); output("PopFront", timeRun(r, A));

  r = new owned InsertRandom(n); output("InsertR", timeRun(r, A));

  r = new owned PopBack(); r.run(A); // clean up
  r = new owned InsertSorted(n); output("InsertSB", timeRun(r, A));

  assert(isSorted(A,linearN));

  r = new owned PopBack(); r.run(A); // clean up
  r = new owned InsertSorted(n); output("InsertSL", timeRun(r,A));
  assert(isSorted(A,linearN));

  r = new owned Remove(n, true); output("RemoveFront", timeRun(r, A));
  r = new owned PushBack(n); r.run(A);
  r = new owned Remove(n, false); output("RemoveBack", timeRun(r, A));

  var l = new LinkedList(int);
  r = new owned ListAppend(n); output("ListAppend", timeRunList(r,l));
  assert(l.length == n);
  r = new owned SumReduceList(n); output("ListReduce", timeRunList(r, l));
  r = new owned ListDestroy(); output("ListDest", timeRunList(r,l));
}
