
record foo {
  fun f() { return 1; }
}
fun f(x : foo) { return 2; }

{
  fun f(x : foo) { return 3; }
  {
    fun f(x : foo) { return 4; }
  }
  var x = foo();
  writeln(x.f());
  writeln(f(x));
}
