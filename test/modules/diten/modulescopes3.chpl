module M1 {
  module M2 {
    module M3 {
      var a = 3.14;
    }
  }
}
module M4 {
  use M1 only ;
  var a = 4;
  proc main() {
    foo();
    writeln(M1.M2.M3.a);
    writeln(a);
  }
  proc foo() {
    writeln(a);
    bar();
  }
  proc bar() {
    // initialize these modules, but keep them out of other function scopes
    use M1.M2.M3;
  }
}
