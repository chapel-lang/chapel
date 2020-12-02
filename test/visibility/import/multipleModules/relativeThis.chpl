module Outer {
  module Sub1 {
    var x: int;
  }
  module Sub2 {
    var y: int;
  }

  import this.Sub1, this.Sub2;

  proc main() {
    writeln(Sub1.x);
    writeln(Sub2.y);
  }
}
