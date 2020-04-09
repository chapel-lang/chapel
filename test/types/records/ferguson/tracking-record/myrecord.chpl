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
  var c: unmanaged C?;
}

proc R.init(x : int = 0, c : unmanaged C? = nil) {
  if debug then writeln("in R.init(", x, ", ", c, ")");
  this.x = x;
  this.c = c;
  this.complete();
  this.setup(x, true);
}

proc R.init=(other:R) {
  if debug then writeln("in R.init=(", other, ")");
  this.complete();
  this.setup(other.x, true);
}

proc ref R.setup(x:int, allow_zero:bool=false) {
  if !allow_zero then assert(x != 0);

  if debug {
    printf("in setup deallocating c=%p ", c);
    writeln(c);
  }

  if this.c then trackFree(this.c!, this.c!.id, this.x);
  delete this.c;
  this.c = nil;

  this.x = x;
  this.c = new unmanaged C(x = x, id = 1+c_counter.fetchAdd(1));
  
  extern proc printf(fmt:c_string, arg:C?);
  if debug {
    printf("in setup allocated c=%p ", c);
    writeln(c);
  }

  trackAllocation(this.c!, this.c!.id, this.x);
}

proc ref R.destroy() {
  if c then trackFree(c!, c!.id, this.x);
  delete c;
  c = nil;
}

proc ref R.increment() {
  assert(x != 0);
  assert(c != nil);
  x += 1;
  c!.x += 1;
}


proc R.deinit() {
  extern proc printf(fmt:c_string, arg:C?);
  if debug {
    printf("in destructor for c=%p ", c);
    writeln("x=", x, " ", c);
  }

  if c then trackFree(c!, c!.id, this.x);
  delete c;
}

proc R.verify() {
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));

  // default initialized records have nil ptr, OK
  if c == nil && x == 0 {
    return;
  }

  if c == nil {
    // any time we set x!=0 the class should be initialized
    // (in R.setup).
    writeln("R.verify failed - no class but x != 0");
  }
  // otherwise, check that R.x == R.c.x
  else if x != c!.x {
    writeln("R.verify failed - got x=", x, " but c.x=", c!.x);
    assert(false);
  }
}

proc =(ref lhs: R, rhs: R) {
  extern proc printf(fmt:c_string, arg:C?);
  extern proc printf(fmt:c_string, arg:C?, arg2:C?);
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

