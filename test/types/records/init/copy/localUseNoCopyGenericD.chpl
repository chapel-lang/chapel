module Bar {
  var globalR = new R(1,2,3);

  record R {
    var x,y,z;
  }
  proc R.init=(other:R){
    x = other.x;
    y = other.y;
    z = other.z;
  }
  proc R.init(x,y,z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  proc R.deinit() {
  }

  proc getter() {
    // compiler inserts a chpl__initCopy, which is flagged as erroneous because
    // we were unable to resolve the default copy initializer from
    // chpl_gen_main.
    return globalR;
  }
}

module Foo {
  record Inner {
    var a:int;
  }

  proc main() {
    use Bar;

    // For 's = r' below, the compiler does actually insert the default copy
    // initializer.
    var r = new R(new Inner(), new Inner(), new Inner());
    var s = r;

    writeln(r);
    writeln(s);
    writeln(getter());
  }
}
