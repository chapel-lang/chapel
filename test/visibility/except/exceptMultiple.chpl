module M1 {
  var a: int = 12;

  var b: bool = true;

  var c: real = 17.5;

  var d = a * 15;

  var f = -11;

}

module M2 {
  use M1 except a, b, f;

  // Ensures that we can specify multiple symbols to ignore
  proc main () {
    writeln(a);
    writeln(b);
    writeln(f);
  }
}
