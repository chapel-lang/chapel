module M1 {
  use M2;

  var a = 1;
}

module M2 {
  use M1;
  var n = a;
}

module M3 {
  proc main {
    use M1, M2;
    writeln(a);
    writeln(n);
  }
}
