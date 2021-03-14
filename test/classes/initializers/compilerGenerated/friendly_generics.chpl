// User requested feature, see github issue #5143, Part 2c
record A {
  type T;
  var a: T;
}

type C = A(int);

var c = new C(1);
// since C is an instantiation of A, we should know the value of T in this case
// and the new call should "just work", but only so long as there is no single
// argument user-defined initializer
writeln(c);
writeln(c.a);
