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
    // Verifies that all modules were properly renamed
    writeln(A.x);
    writeln(B.y);
    writeln(C.z);
  }
}
