// Test for issue #27411: Multiple fields with domain(?) should work
class C {
  var d: domain(?);
  var d2: domain(?);

  proc init() {
    this.d = {1..10};
    this.d2 = {1..20};
  }
}

var c = new C();
writeln(c);
