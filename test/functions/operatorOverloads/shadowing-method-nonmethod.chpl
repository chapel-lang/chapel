module Library {

  record R { var x: int; }

  operator R.+(a: R, b: R) {
    writeln("in Library plus");
    return new R(a.x + b.x);
  }
}

module Variant {
  import Library.R;

  operator +(a: R, b: R) {
    writeln("in Variant plus");
    return new R(a.x + b.x);
  }
}

module Main {
  use Library;

  proc main() {
    use Variant;

    var x = new R(1);
    var y = new R(2);
    var z = x + y;

    assert(z.x == 3);
  }
}
