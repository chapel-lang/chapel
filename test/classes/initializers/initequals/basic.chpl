
record R {
  var x : int;

  proc init(x : int = 0) {
    writeln("Initialize from integer");
    this.x = x;
  }
}

writeln("----- Classic Initialization -----");
{
  var A : R;
  var B = new R(5);
  writeln(A);
  writeln(B);
}
writeln("\n\n");

writeln("----- User-Defined Copy Initialization -----");

proc R.init=(other : R) {
  writeln("classic copy initialization");
  this.x = other.x;
}

{
  var A = new R(10);
  var B = A;
  writeln(A);
  writeln(B);
}
writeln("\n\n");

writeln("----- Init= from integer -----");

proc R.init=(x : int) {
  writeln("init= from integer");
  this.x = x;
}

{
  var A : R = 5;
  writeln(A);
}
