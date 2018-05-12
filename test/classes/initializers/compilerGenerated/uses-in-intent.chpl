
record R {
  var x : int;
  proc init() { writeln("R.init"); }
  proc init(other:R) { this.x = other.x + 1; writeln("R.init(R): ", x); }
  proc deinit() { writeln("R.deinit: ", x); }
}

pragma "use default init"
class Simple {
  var r = new R();
}

writeln("\nSimple zero actuals");
{
  var s = new Simple();
  writeln("s = ", s);
  delete s;
}
writeln("\nSimple var actual");
{
  var r = new R();
  var s = new Simple(r);
  writeln("s = ", s);
  delete s;
}
writeln("\nSimple new actual");
{
  var s = new Simple(new R());
  writeln("s = ", s);
  delete s;
}
