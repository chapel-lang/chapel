module M1 {
  module M2 {
    module M3 {
      var a = 3.14;
    }
  }
}

module M4 {
  var a = 4;
  def main() {
    foo();
    writeln(M1.M2.M3.a);
    writeln(a);
  }
  def foo() {
    writeln(a);
    bar();
  }
  def bar() {
    // initialize these modules, but keep them out of other function scopes
    use M1.M2.M3;
  }
}
