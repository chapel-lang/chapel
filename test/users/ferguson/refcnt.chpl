class RefCount {
  var count:int;
  proc init() {
    writeln("Initializing a RefCount");
    count = 1;
  }
  proc deinit() {
    writeln("Destroying a RefCount");
  }
}

record R {
  var refcnt:unmanaged RefCount? = new unmanaged RefCount();
  proc retain() {
    refcnt!.count += 1;
  }
  proc release() {
    refcnt!.count -= 1;
    if refcnt!.count == 0 {
      delete refcnt;
      refcnt = nil;
    }
  }
  proc deinit() {
    writeln("In ~R() ", refcnt!.count);
    this.release();
  }

}

proc R.init() {
}

proc R.init=(x: R) {
  writeln("In R.init=(R) ", x.refcnt);
  this.refcnt = x.refcnt;
  this.complete();
  this.retain();
}


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

writeln("Before scope");
{
  var r:R;

  test_one(r, false);
}
writeln("After scope");

writeln("Before scope (copy initialization)");
{
  var r:R;
  var x:R = r;

  test_one(x, false);
}
writeln("After scope (copy initialization)");

writeln("Before scope (copy initialization 2)");
{
  var r:R;
  var x = r;

  test_one(x, false);
}
writeln("After scope (copy initialization 2)");


writeln("Before scope (setting global)");
{
  var r:R;

  test_one(r, true);
}
writeln("After scope (setting global)");

writeln("Before scope (begin)");
{
  var r:R;
  sync {
    begin {
      test_one(r, false);
    }
  }
}
writeln("After scope (begin)");

writeln("Finishing");
