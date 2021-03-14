var A: [0..9] int = [i in 1..10] i;
var B: [0..9] int = [i in 1..10] i;

var t1 = (B, 19);
var t2 = (A, 20);

writeln("A is: ", A);
writeln("t1(0) is: ", t1(0));
writeln("t2(0) is: ", t2(0));
writeln();

foo(A, 17);
writeln("A is: ", A);
writeln("t1(0) is: ", t1(0));
writeln("t2(0) is: ", t2(0));
writeln();

bar((A, 18));
writeln("A is: ", A);
writeln("t1(0) is: ", t1(0));
writeln("t2(0) is: ", t2(0));
writeln();

bar(t1);
writeln("A is: ", A);
writeln("t1(0) is: ", t1(0));
writeln("t2(0) is: ", t2(0));
writeln();

bar(t2);
writeln("A is: ", A);
writeln("t1(0) is: ", t1(0));
writeln("t2(0) is: ", t2(0));
writeln();

proc foo(x,y) {
  x(0) = y;
}

proc bar((x,y)) {
  x(0) = y;
}
