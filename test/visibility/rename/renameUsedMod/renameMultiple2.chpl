module First {
  var x: int;
}

module Second {
  var y: bool;
}

module Third {
  var z: string;
}

module User {
  use First as A, Second as B, Third as C;

  proc main() {
    writeln(x);
    writeln(y);
    writeln(z);
    // Verifies that the First module can no longer be used with the original
    // name
    writeln(First.x);
  }
}
