enum myenum {
  ONE=1,
  TWO=2,
  THREE=3
}
use myenum;

var A:[1..3] myenum = [ONE, TWO, THREE];
writeln(A);

var B = A:int;
writeln(B);

var C = B:int; // this useless cast is promoted, makes C = B.
writeln(C);

var D = C:myenum; // demonstrating cast to enum can be promoted
writeln(D);

var F = A:myenum; // results in compilation error
writeln(F);
