/*
   This is an invalid Chapel program.

   M3 contains main().

   M3 uses M1 which uses M2.
     This implies that the partial initialization order is M2, M1, M3.

     This implies an attempt to initialize M2.d before M1.a has been
     initialized.

  Historically this program has failed to type-resolve "because"
  function resolution determined the types of a and d in the same
  order that the program would execute.

  If the type of 'a' is set early enough (e.g. normalize) this program might
  compile successfully.
*/

module M1 {
  use M2;
  var a = 1;
  var b = c;   // OK
}

module M2 {
  use M1;
  var c = 2;
  var d = a;   // Use before definition
}

module M3 {
  use M1, M2;
  proc main {
    writeln((a, b, c, d));
  }
}
