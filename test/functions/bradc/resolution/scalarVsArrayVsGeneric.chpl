def f(i: int,    j: int) {
  writeln("In f(:int, :int)");
}

def f(i: [] int, j: int) {
  writeln("In f(:[] int, :int)");
}

def f(i: int,    j) {
  writeln("In f(:int, :any)");
}

def f(i: [] int, j) {
  writeln("In f(:[] int, :any)");
}

var D = [1..4];
var A: [D] int = [i in D] i;
var B: [D] int = [i in D] i;

f(2,3);
f(2,A);
f(A,2);
f(A,B);

