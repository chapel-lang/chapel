var A: [1..10] int = [i in 1..10] i;
var B: [1..10] int = [i in 1..10] i;

var t1 = (B, 19);
var t2 = (A, 20);

writeln("A is: ", A);
writeln("t1(1) is: ", t1(1));
writeln("t2(1) is: ", t2(1));
writeln();

foo(A, 17);
writeln("A is: ", A);
writeln("t1(1) is: ", t1(1));
writeln("t2(1) is: ", t2(1));
writeln();

bar((A, 18));
writeln("A is: ", A);
writeln("t1(1) is: ", t1(1));
writeln("t2(1) is: ", t2(1));
writeln();

bar(t1);
writeln("A is: ", A);
writeln("t1(1) is: ", t1(1));
writeln("t2(1) is: ", t2(1));
writeln();

bar(t2);
writeln("A is: ", A);
writeln("t1(1) is: ", t1(1));
writeln("t2(1) is: ", t2(1));
writeln();

proc foo(x,y) {
  x(1) = y;
}

proc bar((x,y)) {
  x(1) = y;
}
