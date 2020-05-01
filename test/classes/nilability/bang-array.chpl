// #15080: we would like (an array)! to be compatible with an array
// as much as possible. This comes handy, for example, when we want
// a non-nilable array field and initialize it by building a nilable array
// then passing (the nilable array)! to the enclosing class's initializer.

class C {}
var a: [1..1] owned C?;
a[1] = new C();
foo(a!);

proc foo(arrayArg: [] C) {
  writeln(arrayArg);
}
