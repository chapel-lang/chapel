class C {
  var bar: float;

  def foo(x) {
    return x+0.1;
  }

  iterator iterate(numValues, startPoint = 0.0): float {
    var nextVal = startPoint:float;
    for i in 1..numValues {
      nextVal = foo(nextVal);
      yield nextVal;
    }
    bar = nextVal;
  }
}

var myC = C();

for i in myC.iterate(10) {
  writeln("i is: ", i);
}
writeln("myC.bar is: ", myC.bar);
writeln();


for j in myC.iterate(10, 0.5) {
  writeln("j is: ", j);
}
writeln("myC.bar is: ", myC.bar);

