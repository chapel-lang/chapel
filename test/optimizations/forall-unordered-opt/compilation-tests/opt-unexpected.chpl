config const N=1000;
config const M=10000;
config const choice=true;

// Explicit main included to reduce test maintenance
proc main() { }

proc set_then_atomic() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;
  var x:atomic int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex.localAccess[i]];
    x.write(1); // don't optimize -- other thread could assume tmp updated
                // after x is set
  }
}
set_then_atomic();

proc set_then_sync() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;
  var x:sync int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex.localAccess[i]];
    x = 1;      // don't optimize -- other thread could assume tmp updated
                // after x is set
  }
}
set_then_sync();

proc set_then_single() {
  var A: [0..#M] int = 0..#M;
  var rindex: [0..#N] int;
  var tmp: [0..#N] int;
  var x:single int;

  forall i in 0..#N {
    tmp.localAccess[i] = A[rindex.localAccess[i]];
    x = 1;      // don't optimize -- other thread could assume tmp updated
                // after x is set
  }
}
set_then_single();


proc histo_spin() {
  var A: [0..#M] atomic int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    A[r].add(1);
    while B[r].read() == 0 { }
    // don't optimize -- could be waiting on another task
    // to update B based on the A update
  }
}
histo_spin();

proc histo_spin2() {
  var A: [0..#M] atomic int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  forall r in rindex {
    A[r].add(1);
    if r == 1 {
      while B[r].read() == 0 { }
    }
    // don't optimize -- could be waiting on another task
    // to update B based on the A update
  }
}
histo_spin2();

proc refIdentity(ref x) ref {
  return x;
}

proc updates_local_atomic() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var x: atomic int;
    x.add(1);
  }
}
updates_local_atomic();

proc updates_local_atomic2() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var x: atomic int;
    ref refx = refIdentity(x);
    refx.add(1);
  }
}
updates_local_atomic2();

proc updates_local() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var y: int = 7;
    var x: int;
    x = y;
  }
}
updates_local();

proc updates_local2() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var y: int = 7;
    var x: int;
    ref refx = refIdentity(x);
    refx = y;
  }
}
updates_local2();

proc updates_local3() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var y: int = 7;
    var x: int;
    ref refy = refIdentity(y);
    ref refx = refIdentity(x);
    refx = refy;
  }
}
updates_local3();

proc updates_local4() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var y: int = 7;
    var x: int;
    ref refy = refIdentity(y);
    ref refx = refIdentity(x);
    forall r in rindex with (ref refx, ref refy) {
      refy += 1;
      refx += 1;
    }
    refx = refy;
  }
}
updates_local4();

class MyClass {
  var x: atomic int;
}

proc local_instance1() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var instance = new unmanaged MyClass();
    instance.x.add(1);
    delete instance;
  }
}
local_instance1();

proc local_instance2() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var instance = new owned MyClass();
    instance.x.add(1);
  }
}
local_instance2();


record MyRecord {
  var x: atomic int;
}

proc local_record() {
  var rindex: [0..#N] int;

  forall r in rindex {
    var r = new MyRecord();
    r.x.add(1);
  }
}
local_record();


proc mini_lock() {
  var myLock: atomic int;

  forall r in 1..M {
    // Acquire the lock
    while myLock.compareAndSwap(0, 1) {
      //chpl_task_yield();
    }
    // do something meaningful

    // release the lock
    myLock.sub(1);
  }
}
mini_lock();

proc doCmpXchng(ref myLock: atomic int) {
  return myLock.compareAndSwap(0, 1);
}


proc lock(ref myLock: atomic int) {
  while doCmpXchng(myLock) == false {
    //chpl_task_yield();
  }
}

proc mini_lock2() {
  var myLock: atomic int;

  forall r in 1..M {
    // Acquire the lock
    lock(myLock);
    // do something meaningful

    // release the lock
    myLock.sub(1);
  }
}
mini_lock2();

proc mini_lock3() {
  var myLock: atomic int;

  forall r in 1..M {
    // Acquire the lock
    var x = 1;
    while x < 10000 {
      while myLock.compareAndSwap(0, 1) {
        //chpl_task_yield();
      }
      x += 1;
    }
    // do something meaningful

    // release the lock
    myLock.sub(1);
  }
}
mini_lock3();


proc lockRecursive(ref myLock: atomic int) {
  if myLock.compareAndSwap(0, 1) then
    return;

  lockRecursive(myLock);
}

proc mini_lock4() {
  var myLock: atomic int;

  forall r in 1..M {
    // Acquire the lock
    lockRecursive(myLock);
    // do something meaningful

    // release the lock
    myLock.sub(1);
  }
}
mini_lock4();

proc mini_lock5() {
  var myLock: int;
  var myAtomic: atomic int;

  forall r in 1..M with (ref myLock) {
    // Acquire the lock
    // This code is wrong and racy, but the analysis
    // should nonetheless detect it as "blocking"
    while myLock == 1 {
      atomicFence();
      if myLock == 0 {
        myLock = 1;
        break;
      }
    }
    myAtomic.add(1);

    // do something meaningful

    // release the lock
    myAtomic.sub(1);
  }
}
mini_lock5();


// This one would probably be OK but the analysis is
// conservative enough to reject it
proc mini_sync() {
  var count: atomic int;
  var myLock: sync int;

  forall r in 1..M {
    // Acquire the lock
    myLock = 1;
    // Release the lock
    myLock = 0;
    // release the lock
    count.add(1);
  }
}
mini_sync();

// similar to mini_sync
proc mini_single() {
  var count: atomic int;
  var myLock: single int;

  forall r in 1..M {
    // Acquire the lock
    myLock = 1;
    // Release the lock
    myLock = 0;
    // release the lock
    count.add(1);
  }
}
mini_single();

// this case uses task-local storage and so shouldn't be optimized
proc tls_hazard_atomic_conditional() {
  var taskCounter: atomic int;
  var iterationsForTask:[0..M] atomic int;
  forall i in 1..N with (var id = taskCounter.fetchAdd(1)) {
    if 0 <= id && id <= M {
      iterationsForTask[id].add(1);
      assert(iterationsForTask[id].read() == 1);
      // do something useful...
      iterationsForTask[id].sub(1);
    }
  }
}
tls_hazard_atomic_conditional();

proc tls_hazard_atomic() {
  var taskCounter: atomic int;
  var iterationsForTask:[0..M] atomic int;
  forall i in 1..N with (var id = taskCounter.fetchAdd(1)) {
    iterationsForTask[id].add(1);
    assert(iterationsForTask[id].read() == 1);
    // do something useful...
    iterationsForTask[id].sub(1);
  }
}
tls_hazard_atomic();

proc tls_hazard_atomic_thread_id() {
  extern proc chpl_task_getId():chpl_taskID_t;
  var iterationsForTask:[0..M] atomic int;
  forall i in 1..N {
    var id = chpl_task_getId():int;
    iterationsForTask[id].add(1);
    assert(iterationsForTask[id].read() == 1);
    // do something useful...
    iterationsForTask[id].sub(1);
  }
}
tls_hazard_atomic_thread_id();

record buffer1 {
  var b: [1..64] int;
  var cnt: int;

  proc init() {
    b = for i in 1..64 do 0;
    cnt = 0;
  }
  proc init=(other: buffer1) {
    this.b = for i in 1..16 do other.b[i];
    this.cnt = other.cnt;
  }


  inline proc enqueue(i:int) {
    b[cnt] = i;
    cnt += 1;
    if cnt == b.size {
      //flushBuffer(b);
      cnt = 0;
    }
  }
}

proc =(ref lhs:buffer1, rhs:buffer1) {
  for i in 1..64 do
    lhs.b[i] = rhs.b[i];

  lhs.cnt = rhs.cnt;
}

proc tls_hazard_buffer1() {
  var taskCounter: atomic int;
  var perTaskBuff: [0..M] buffer1;

  forall i in 1..N with (var id=taskCounter.fetchAdd(1)) {
    perTaskBuff[id].enqueue(i);
  }
}
tls_hazard_buffer1();

record buffer2 {
  var b: [1..64] int;
  var cnt: int;

  proc init() {
    b = for i in 1..64 do 0;
    cnt = 0;
  }
  proc init=(other: buffer1) {
    this.b = for i in 1..16 do other.b[i];
    this.cnt = other.cnt;
  }
  proc init=(other: buffer2) {
    this.b = other.b;
    this.cnt = other.cnt;
  }

  inline proc enqueue(i:int) {
    b[cnt] = i;
    if cnt == b.size-1 {
      //flushBuffer(b);
      cnt = -1;
    }
    cnt += 1;
  }
}

proc =(ref lhs:buffer2, rhs:buffer2) {
  for i in 1..64 do
    lhs.b[i] = rhs.b[i];

  lhs.cnt = rhs.cnt;
}
proc tls_hazard_buffer2() {
  var taskCounter: atomic int;
  var perTaskBuff: [0..M] buffer2;

  forall i in 1..N with (var id=taskCounter.fetchAdd(1)) {
    perTaskBuff[id].enqueue(i);
  }
}
tls_hazard_buffer2();


record buffer3 {
  var deferredSignal: int;
  var signal: atomic int;
  var happensBeforeSignal: atomic int;
}

proc tls_signalling() {
  var taskCounter: atomic int;
  var perTaskBuff: [0..M] buffer3;


  sync {
    begin {
      // Check that signal implies happensBeforeSignal
      for i in 0..M {
        perTaskBuff[i].signal.waitFor(1);
        assert(perTaskBuff[i].happensBeforeSignal.read() >= 1);
      }
    }

    forall i in 1..N with (var id=taskCounter.fetchAdd(1)) {
      ref tls = perTaskBuff[id];
      if tls.deferredSignal then
        tls.signal.add(1);

      tls.deferredSignal = 1;
      tls.happensBeforeSignal.add(1, order=memoryOrder.relaxed);
    }

    // Complete all of the deferred signals
    for i in 0..M {
      if perTaskBuff[i].deferredSignal then
        perTaskBuff[i].signal.add(1);
        perTaskBuff[i].deferredSignal = 0;
    }
  }
}
tls_signalling();

proc tls_signalling2() {
  var taskCounter: atomic int;
  var perTaskBuff: [0..M] buffer3;

  sync {
    begin {
      // Check that signal implies happensBeforeSignal
      for i in 0..M {
        perTaskBuff[i].signal.waitFor(1);
        assert(perTaskBuff[i].signal.read() == 1);
      }
    }

    forall i in 1..N with (var id=taskCounter.fetchAdd(1)) {
      ref tls = perTaskBuff[id];
      if tls.deferredSignal {
        tls.signal.add(1);
      }

      tls.deferredSignal = 1;
    }

    // Complete all of the deferred signals
    for i in 0..M {
      if perTaskBuff[i].deferredSignal then
        perTaskBuff[i].signal.add(1);
    }
  }
}
tls_signalling2();

