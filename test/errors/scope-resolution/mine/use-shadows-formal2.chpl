module M0 {
  var b: int;
  var b: real;
}

module M1 {
  public use M0 only b as a;
}

module M2 {
  proc main {
    foo(3.0);
  }
  proc foo(a: real) {
    use M1;
    writeln(a);
  }
}
