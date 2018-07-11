/* This test record asserts that the record does not
   change locality without a record copy function or = being called.
 */

config const debug = false;

record R {
  var x:int;
  var home:locale = here;
}


proc ref R.init(x:int, allow_zero:bool=false) {
  if !allow_zero then assert(x != 0);
  assert(this.home == this.locale);
  this.x = x;
}

proc ref R.destroy() { }

proc ref R.increment() {
  assert(this.home == this.locale);
  assert(x != 0);
  x += 1;
}


proc R.deinit() {
  if debug then writeln("In record destructor");
  assert(this.home == this.locale);
}

proc ref R.verify() {
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));

  if this.home != this.locale {
    writeln("R.verify failed - got this.locale=", this.locale, " but expected this.home=", this.home);
    assert(false);
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  assert(arg.home == arg.locale);

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  // allow auto copies of default init'd record
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
  assert(arg.home == arg.locale);

  var ret: R;

  ret.init(x = arg.x);

  if debug {
    writeln("leaving init copy");
  }

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  // both LHS and RHS should be initialized.
  assert(lhs.home == lhs.locale);
  assert(rhs.home == rhs.locale);

  lhs.init(x = rhs.x);

  if debug {
    writeln("leaving assign");
  }
}

proc verify() {
}

