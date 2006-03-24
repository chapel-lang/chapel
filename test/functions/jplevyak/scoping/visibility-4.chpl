record bar {
  var x : integer;
}
record foo : bar {
  var y : integer;
}
fun f(x : foo) { return 2; }

{
  fun f(x : bar) { return 3; }
  {
    fun f(x : foo) { return 4; }
  }
  var x = foo();
  writeln(f(x));
}
