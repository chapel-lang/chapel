/* This test record asserts that any record passed to 
   auto copy/ init copy /assign has been initialized.
 */

config const debug = false;

pragma "ignore noinit"
record R {
  var x: int = 0;
  var canary: int = 42;
}


proc ref R.init(x:int, allow_zero:bool=false) {
  if !allow_zero then assert(x != 0);
  this.x = x;
  
  if canary != 42 {
    writeln("init with uninitialized record!");
    assert(canary == 42);
  }

}

proc ref R.destroy() { }

proc ref R.increment() {
  assert(x != 0);
  
  if canary != 42 {
    writeln("increment with uninitialized record!");
    assert(canary == 42);
  }

  x += 1;
}


proc R.~R() {
  if debug then writeln("In record destructor");

  if canary != 42 {
    writeln("record destruction with uninitialized record!");
    assert(canary == 42);
  }
}

proc ref R.verify() {
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));

  if canary != 42 {
    writeln("R.verify failed - got canary=", canary, " but expected 42");
    assert(false);
  }
}

// We'd like this to be by ref, but doing so leads to an internal
// compiler error.  See
// $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  if arg.canary != 42 {
    writeln("autoCopy with uninitialized record!");
    assert(arg.canary == 42);
  }


  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  // allow copies of a default-initialized record.
  ret.init(x = arg.x, true);

  if debug {
    writeln("leaving auto copy");
  }

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  if arg.canary != 42 {
    writeln("initCopy with uninitialized record!");
    assert(arg.canary == 42);
  }

  var ret: R;

  ret.init(x = arg.x);

  if debug {
    writeln("leaving init copy");
  }

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  // both LHS and RHS should be initialized.
  if lhs.canary != 42 {
    writeln("= with uninitialized lhs!");
    assert(lhs.canary == 42);
  }
  if rhs.canary != 42 {
    writeln("= with uninitialized rhs!");
    assert(rhs.canary == 42);
  }

  lhs.init(x = rhs.x);

  if debug {
    writeln("leaving assign");
  }
}

proc verify() {
}

