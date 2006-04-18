record bar {
  var x : int;
}
record foo : bar {
  var y : int;
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
