module M1 {
  var a: int;
}

module M2 {
  def main {
    foo(3.0);
  }
  def foo(a: real) {
    var a: complex = 1 + 2i;
    use M1;
    writeln(a);
  }
}
