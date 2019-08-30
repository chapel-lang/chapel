
record R {
  type T;
  param p : T;
}

proc foo(x : R(int)) {
  writeln("int version");
  writeln(x.type:string);
  writeln();
}

proc foo(x : R(real)) {
  writeln("real version");
  writeln(x.type:string);
  writeln();
}

proc foo(x : R) {
  writeln("generic version");
  writeln(x.type:string);
  writeln();
}

foo(new R(int, 100));
foo(new R(real, 123.456));
foo(new R(string, "hello"));

writeln("--------------------");

record P {
  type T;
  param x : T;
  type U;
  param y : U;

  proc init(type T, param x : T, type U, param y : U) {
    this.T = T;
    this.x = x;
    this.U = U;
    this.y = y;
  }

  proc init(param x : ?T, param y : ?U) {
    this.T = T;
    this.x = x;
    this.U = U;
    this.y = y;
  }
}

proc bar(x : P(int, U=int)) {
  writeln("bar(P(int,U=int))");
  writeln(x.type:string);
  writeln();
}

proc bar(x : P(U=bool)) {
  writeln("bar(P(U=bool))");
  writeln(x.type:string);
  writeln();
}

proc bar(x : P) {
  writeln("bar(P)");
  writeln(x.type:string);
  writeln();
}

bar(new P(100, 100));         // P(int,U=int)
bar(new P(100, false));       // P(U=bool)
bar(new P("hello", true));    // P(U=bool)
bar(new P("hello", "world")); // P

writeln("--------------------");

proc baz(x : P(U=real, y=1.0)) {
  writeln("baz(P(U=real,y=1.0))");
  writeln(x.type:string);
  writeln();
}

proc baz(x : P) {
  writeln("baz(P)");
}

// TODO: Compiler currently generates a call to '==' when comparing the desired
// param value against the field's instantiated value. This call fails to
// resolve when the types are different. Once this bug is fixed, this test
// should add more overloads of 'baz' with different param instantiations.
baz(new P(123, 1.0));
baz(new P(123, 99.99));

writeln("--------------------");

class C {
  type T;
  param n : int;
  var x : T;
}

proc foobar(x : owned C(int)) {
  writeln("foobar(owned C(int))");
  writeln(x.type:string, ": ", x);
  writeln();
}

proc foobar(x : owned C(real)) {
  writeln("foobar(owned C(real))");
  writeln(x.type:string, ": ", x);
  writeln();
}

proc foobar(x) {
  writeln("foobar(?)");
  writeln(x.type:string, ": ", x);
  writeln();
}

foobar(new owned C(int, 4));
foobar(new owned C(int, 100));
foobar(new owned C(real, 3));
foobar(new owned C(string, 10));

proc barfoo(x : owned C(n=4)) {
  writeln("barfoo(owned C(n=4))");
  writeln(x.type:string, ": ", x);
  writeln();
}

proc barfoo(x : owned C(n=100)) {
  writeln("barfoo(owned C(n=100))");
  writeln(x.type:string, ": ", x);
  writeln();
}

proc barfoo(x) {
  writeln("barfoo(?)");
  writeln(x.type:string, ": ", x);
  writeln();
}

barfoo(new owned C(string, 4));
barfoo(new owned C(int, 4));
barfoo(new owned C(int, 100));
barfoo(new owned C(real, 1));
