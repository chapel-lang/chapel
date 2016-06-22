module M1 {
  var a: int = 12;

  var b: bool = true;
}

module M2 {
  var c: uint = 19;

  var b: real = 14.732;

}

module M3 {
  use M1 except b;
  use M2;

  // Ensures that if we've specified to ignore a symbol in one module, a symbol
  // with the same name in a different module will not conflict or be ignored
  proc main() {
    writeln(a);
    writeln(b);
    writeln(c);
  }
}
