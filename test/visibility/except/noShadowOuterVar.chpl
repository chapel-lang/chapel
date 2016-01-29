module M1 {
  var a: int = 12;

  var b: bool = true;

}

module M2 {
  var b: bool = false;

  // Ensures that the outer variable b is not shadowed by the ignored variable
  // in the module we used.
  proc main() {
    use M1 except b;
    writeln(b);
  }
}
