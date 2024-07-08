module M1 {
  record R { }
  proc R.a { }
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
