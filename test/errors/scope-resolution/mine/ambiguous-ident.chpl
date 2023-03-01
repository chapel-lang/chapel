module M1 {
  var x: int;
}

module M2 {
  var x: real;
}

module M {
  use M1, M2;
  proc main() {
    x;
    x;
  }
}
