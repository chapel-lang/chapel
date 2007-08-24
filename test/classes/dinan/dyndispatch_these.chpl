class A {
  var n = 0;
  
  def these() {
    for i in 1..n do
      yield i;
  }
}

class B:A {
  def these() {
    for i in 1..n by -1 do
      yield i;
  }
}

var a = A(4);
var b = B(4);

writeln("A:");
for i in a do writeln(i);
  
writeln("\nB:");
for i in b.these() do writeln(i);
