
config param case : int = 0;

record Typeless {
  var x;
}

record BadUserInit {
  type T;
  var x : T;

  proc init(x : ?T) {
    this.T = T;
    this.x = x;
  }
}

proc main() {
  if case == 0 {
    // Using a typeless field
    type TI = Typeless(int);
    var x = new TI(5);
    writeln(x);

  } else if case == 1 {
    // Incompatible user-defined initializer
    type BI = BadUserInit(int);
    var x = new BI(5);
    writeln(x);

  } else {
    compilerError("Should not reach this branch");
  }
}
