class C {
  var bar: real;

  proc foo(x) {
    return x+0.1;
  }

  iter iterate(numValues, startPoint = 0.0): real {
    var nextVal = startPoint:real;
    for i in 1..numValues {
      nextVal = foo(nextVal);
      yield nextVal;
    }
    bar = nextVal;
  }
}

var myC = new unmanaged C();

for i in myC.iterate(10) {
  writeln("i is: ", i);
}
writeln("myC.bar is: ", myC.bar);
writeln();


for j in myC.iterate(10, 0.5) {
  writeln("j is: ", j);
}
writeln("myC.bar is: ", myC.bar);

delete myC;
