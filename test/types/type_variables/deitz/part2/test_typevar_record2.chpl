record foo {
  type eltType;
  var x : eltType;
  def print() {
    writeln(x);
  }
}

var f : foo of string;
f.x = "hello world";
writeln(f.x);

var g : foo of int;
g.x = 2;
writeln(g.x);
