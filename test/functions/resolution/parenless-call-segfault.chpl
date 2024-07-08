// This was a .future test for #16128
// unimplemented feature: creating a fcf using qualified access

module M {
  proc y() {
    writeln("Executing y");
    return 33;
  }
}

module N {
  proc main() {
    use M;
    var fcf = M.y;
    writeln(fcf());
    M.y();
    writeln("Success");
  }
}
