module M {
  use L;
  var y: int = 33;
  record R {
  }
}

module L {
  var x: int = 45;
}

module O {
  import M.R;
  proc R.foo() {
    writeln(x + y);
  }
  proc main() {
    var myR: R;
    myR.foo();
  }
}
