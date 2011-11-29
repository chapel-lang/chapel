module M1 {
  var a: int;
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
