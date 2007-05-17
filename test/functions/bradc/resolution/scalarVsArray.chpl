def f(i: int) {
  writeln("In foo(:int)");
}

def f(i: [] int) {
  writeln("In foo(:[] int)");
}

var D = [1..4];
var A: [i in D] int = i;

f(2);
f(A);
