module M1 {
  use M2, Time;
  class C {
    var field: int;
  }
  var a:C = new C(1);
}

module M2 {
  use M1;
  var n:C = a;
}

module M3 {
  proc main {
    use M1, M2;
    writeln(a);
    writeln(n);
  }
}
