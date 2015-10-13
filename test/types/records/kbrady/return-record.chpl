config const debugAssign = false;

module foo {
  class helper {
    var y: int;
  }
  pragma "ignore noinit"
  record Zed {
    var x: helper;

    proc Zed(x) {
      writeln("in Zed(",x,")");
      this.x = new helper(x);
    }

    proc ~Zed() {
      writeln("in ~Zed(",x,")");
      delete this.x; this.x = nil;
    }
  }

  pragma "init copy fn"
    proc chpl__initCopy(const ref rhs: Zed) {
    writeln("in chpl__initCopy(", rhs.x, ":Zed)");
    var result: Zed;
    result.x = if rhs.x then new helper(rhs.x.y) else nil;
    return result;
  }

  proc =(ref lhs: Zed, rhs: Zed) {
    if debugAssign then writeln("in =(Zed(",lhs.x,"), Zed(",rhs.x,"))");
    else writeln("in =(Zed, Zed)");
    delete lhs.x;
    lhs.x = new helper(rhs.x.y + 1);
  }

  /////////////////////////////////

  proc baz() {
    writeln("in baz()");
    var ret = new Zed(1);
    return ret;
  }

  // This part of the test does not work, because we can't declare explicit
  // return types on functions where the return type performs memory
  // management.  This is because normalize.cpp breaks AMM rules
  // by inserting an uninitialized return-value variable that then updated
  // through assignment :-O.  Fixing that is slightly beyond the scope of the
  // AMM rework, because it involves reworking all of the return type inference
  // and checking machinery.
  proc gob() : Zed {
    writeln("in gob()");
    var ret = new Zed(1);
    return ret;
  }

  /////////////////////////////////

  proc try_baz() {
    writeln("type inferred");
    writeln("-------------");
    var z1 = baz();
    writeln(z1);
  }

  proc try_gob() {
    writeln("\ntype specified");
    writeln("--------------");
    var z2 = gob();
    writeln(z2);
  }

  /////////////////////////////////

  proc main() {
    try_baz();
    try_gob();
  }
}
