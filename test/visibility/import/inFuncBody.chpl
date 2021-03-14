module A {
  var x: int;
}

module User {
  proc main() {
    import A;

    writeln(A.x);
  }
}
