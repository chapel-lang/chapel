module M1 {
  use M2;
  var a = 1;
  var b = c;
}

module M2 {
  use M1;
  var c = 2;
  var d = a;
}

module M3 {
  use M1, M2;
  proc main {
    writeln((a, b, c, d));
  }
}
