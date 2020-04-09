module A {
  module B {
    var x: int;
  }
  var y: int;
}
module User {
  import A.{B, y};

  proc main() {
    writeln(B.x);
    writeln(y);
  }
}
