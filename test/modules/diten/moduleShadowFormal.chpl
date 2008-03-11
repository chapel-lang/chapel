module M1 {
  var a: int;
}

module M2 {
  def main {
    foo(3.0);
  }
  def foo(a: real) {
    use M1;
    writeln(a);
  }
}
