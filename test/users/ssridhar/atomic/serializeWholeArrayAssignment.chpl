config var n: int = 8;

var D: domain(1) = [1..n];

var A: [D] int;
var B: [D] int;

def main() {
  for i in D do A(i) = i;

  writeln("A is: ", A);

  atomic serial true do B = A;

  writeln("B is: ", B);
}