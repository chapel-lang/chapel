/* This test record keeps a class pointer hanging off of
   a field an tracks allocation/free operations on that
   class pointer.
 */

use SysBasic;
use Tracking;

config const debug = false;
config param allocateAlways = true;

var c_counter: atomic int;

class C {
  var id: int = 0;
  var x: int = 0;
}

record R {
  var x: int = 0;
  var c: C;
}

proc ref R.setup(x:int, allow_zero:bool=false) {
  if !allow_zero then assert(x != 0);

  if debug {
    printf("in setup deallocating c=%p ", c);
    writeln(c);
  }

  if this.c then trackFree(this.c, this.c.id, this.x);
  delete this.c;
  this.c = nil;

  this.x = x;
  this.c = new C(x = x, id = 1+c_counter.fetchAdd(1));
  
  extern proc printf(fmt:c_string, arg:C);
  if debug {
    printf("in setup allocated c=%p ", c);
    writeln(c);
  }

  trackAllocation(c, c.id, this.x);
}

proc ref R.destroy() {
  if c then trackFree(c, c.id, this.x);
  delete c;
  c = nil;
}

proc ref R.increment() {
  assert(x != 0);
  assert(c != nil);
  x += 1;
  c.x += 1;
}


proc R.~R() {
  extern proc printf(fmt:c_string, arg:C);
  if debug {
    printf("in destructor for c=%p ", c);
    writeln("x=", x, " ", c);
  }

  if c then trackFree(c, c.id, this.x);
  delete c;
}

proc ref R.verify() {
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));

  // default initialized records have nil ptr, OK
  if c == nil && x == 0 then return;

  if c == nil {
    // any time we set x!=0 the class should be initialized
    // (in R.setup).
    writeln("R.verify failed - no class but x != 0");
  }
  // otherwise, check that R.x == R.c.x
  if x != c.x {
    writeln("R.verify failed - got x=", x, " but c.x=", c.x);
    assert(false);
  }
}

// We'd like this to be by ref, but doing so leads to an internal
// compiler error.  See
// $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  extern proc printf(fmt:c_string, arg:C);
  extern proc printf(fmt:c_string, arg:C, arg2:C);
  if debug {
    printf("in auto copy from arg.c=%p ", arg.c);
    writeln("x=", arg.x, " ", arg.c);
  }

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  // allow copies of default initialized record
  if allocateAlways || arg.x!=0 || arg.c!=nil {
    ret.setup(x = arg.x, true);
  }

  if debug {
    printf("leaving auto copy from arg.c=%p to ret.c=%p ", arg.c, ret.c);
    writeln(arg.c, ret.c);
  }

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  extern proc printf(fmt:c_string, arg:C);
  extern proc printf(fmt:c_string, arg:C, arg2:C);
  if debug {
    printf("in init copy from arg.c=%p ", arg.c);
    writeln(arg.c);
  }

  var ret: R;

  ret.setup(x = arg.x, true);

  if debug {
    printf("leaving init copy from arg.c=%p to ret.c=%p ", arg.c, ret.c);
    writeln(arg.c, ret.c);
  }

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  extern proc printf(fmt:c_string, arg:C);
  extern proc printf(fmt:c_string, arg:C, arg2:C);
  if debug {
    printf("in assign lhs = rhs rhs.c is %p ", rhs.c);
    writeln(rhs.c);
  }

  lhs.setup(x = rhs.x, true);

  if debug {
    printf("leaving assign lhs.c %p = rhs.c %p ", lhs.c, rhs.c);
    writeln(lhs.c, rhs.c);
  }
}

proc verify() {
  checkAllocations();
}

