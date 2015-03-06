config const debugAssign = false;

module foo {
  class helper {
    var y: int;
  }
  record Zed {
    var x: helper;

    proc Zed(x) {
      writeln("in Zed(",x,")");
      this.x = new helper(x);
    }

    proc ~Zed() {
      writeln("in ~Zed(",x,")");
      delete this.x;
    }
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
