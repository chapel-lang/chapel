/*
   This is an invalid Chapel program.

   M3 contains main().

   M3 uses M1 which uses M2.
     This implies that the partial initialization order is M2, M1, M3.

     This implies an attempt to initialize M2.n before M1.a has been
     initialized.

  Historically this program has failed to type-resolve "because"
  function resolution determined the types of a and n in the same
  order that the program would execute.

  Changes to certain simple cases for resolution in Feb 2017 mean
  that the types of M1.a and M2.n are known sooner and so the
  program has started to compile.   For this case it happens to
  produce stable output but this is not assured.
*/

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
  proc main() {
    use M1, M2;

    writeln(a);
    writeln(n);
  }
}
