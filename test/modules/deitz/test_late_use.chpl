module M1 {
  var x: int = 4;
}

module M2 {
  proc main {
    writeln(x);
    use M1;
  }
}
