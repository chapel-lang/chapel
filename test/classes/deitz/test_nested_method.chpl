class C {
  var x : int = 2;
  fun foo() {
    var y : int = x;
    writeln(y);
    fun bar() {
      writeln(x + y);
    }
    bar();
  }
}

var c : C = C();

writeln(c);
c.foo();
