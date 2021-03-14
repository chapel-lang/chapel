module Outer {
  module Sub1 {
    var x: int;
  }
  module Sub2 {
    var y: int;
  }

  module Sub3 {
    import super.Sub1, super.Sub2;

    proc main() {
      writeln(Sub1.x);
      writeln(Sub2.y);
    }
  }
}
