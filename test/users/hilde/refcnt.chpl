// refcnt.chpl
//
// Test extracted from users/ferguson/refcnt.chpl
//
// To show specifically, that bad things happen if a record var declared in the
// caller goes away before the task function completes.
//

class RefCount {
  var count:int;
  proc RefCount() {
    writeln("Initializing a RefCount");
    count = 1;
  }
  proc ~RefCount() {
    writeln("Destroying a RefCount");
  }
}

record R {
  var refcnt = new RefCount();
  proc retain() {
    refcnt.count += 1;
  }
  proc release() {
    refcnt.count -= 1;
    if refcnt.count == 0 {
      delete refcnt;
      refcnt = nil;
    }
  }
  proc ~R() {
    writeln("In ~R() ", refcnt.count);
    this.release();
  }

}

pragma "init copy fn"
proc chpl__initCopy(x: R) {
  writeln("In R initCopy ", x.refcnt);
  x.retain();
  return x;
}
/*
proc chpl__autoCopy(x: R) {
  writeln("In R autoCopy ", x.refcnt);
  x.retain();
  return x;
}
*/


proc =(ref ret:R, x:R) {
  // retain then release.
  writeln("Starting R assign ", ret.refcnt, x.refcnt);
  x.retain();
  ret.release();
  ret.refcnt = x.refcnt;
  writeln("Done R assign ", ret.refcnt);
}

proc R.routine() {
  writeln("In R.routine ", refcnt);
}

writeln("Making global");
var globalR:R;
writeln("Done making global");

proc test_two(x:R, setglobal:bool) {
  writeln("In test_two ", x.refcnt);
  if setglobal then globalR = x;
}


proc test_one(x:R, setglobal:bool) {
  writeln("Starting test_one ", x.refcnt);
  x.routine();
  test_two(x, setglobal);
  writeln("Done test_one ", x.refcnt);
}

writeln("Before scope (begin)");
var flag:sync bool;
sync {
  proc call_test_one() {
    // This R exists only within the scope of this function.  It has to be
    // captured by a begin so its lifetime is extended to the end of the task
    // function.
    var r:R;
    begin {
      // Force this to run only after r in calling scope goes away.
      var ready = flag;
      test_one(r, false);
    }
  }
  call_test_one();
  flag = true;
}
writeln("After scope (begin)");

