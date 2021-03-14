var D = {1..4};
var A:[D] int = 1;

proc f() {
  writeln("Nothing to see here");
  return A;
}

writeln(A);
var B:f().type;
writeln(B);

D = {1..2};
writeln(A);
writeln(B);
