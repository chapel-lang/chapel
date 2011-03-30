class A {
  var n = 0;
  
  iter these() {
    for i in 1..n do
      yield i;
  }
}

class B:A {
  iter these() {
    for i in 1..n by -1 do
      yield i;
  }
}

var a = new A(4);
var b = new B(4);

writeln("A:");
for i in a do writeln(i);
  
writeln("\nB:");
for i in b.these() do writeln(i);
