// The operations marked with /// should result in a warning
// (or, in the future, an error).

proc test1() {
  writeln("test2");
  var b$: sync int = 1;
  var c$: sync int = 1;

  var a = b$ + c$; /// warning

  writeln("a = ", a, " : ", a.type:string);
}
test1();

proc test2() {
  writeln("test2");
  var x$: sync int = 1;

  x$ += 2; /// warning

  writeln("x$ = ", x$.readFF(), " : ", x$.type:string);
}
test2();

// not tested here:
//  proc f(in arg)
//  proc f(in arg:int)
// because this results in backend compiler errors before deprecation

proc f(arg: int) {
  writeln("in f arg = ", arg, " : ", arg.type:string);
}
proc test3() {
  writeln("test3");
  var x$: sync int = 1;

  f(x$); /// warning

  x$.writeEF(1);
  writeln("x$ = ", x$.readFF(), " : ", x$.type:string);
}
test3();

proc test4() {
  writeln("test4");
  var x$: sync int = 1;

  var b = x$; /// warning

  writeln("b = ", b, " : ", b.type:string);
}
test4();

proc test5() {
  writeln("test5");
  var a$: sync int = 1;
  a$.readFE();
  var b$: sync int = 1;

  a$ = b$; /// warning

  b$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test5();

proc test6() {
  writeln("test6");
  var a$: sync int = 1;

  var b = a$; /// warning

  a$ = 1;
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test6();

proc test7() {
  writeln("test7");
  var a$: sync int = 1;

  var b$: sync int = a$; /// warning

  a$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test7();

proc test8() {
  writeln("test8");
  var a$: sync int = 1;

  var b: int = a$; /// warning

  a$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test8();

proc test9() {
  writeln("test9");
  var a$: sync int = 1;
  a$.readFE();
  a$.writeEF(1);

  var b: int = a$; /// warning

  a$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test9();

proc test10() {
  writeln("test10");
  var a$: sync int = 1;
  a$.readFE();

  a$ = 1; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
}
test10();

proc test11() {
  writeln("test11");
  var a$: sync int = 1;

  a$; /// warning
}
test11();

proc test12() {
  writeln("test12");
  var A: [1..2] sync int;
  writeln(A.domain); // to avoid split init

  A = 1; /// warning
}
test12();

proc test13() {
  writeln("test13");
  var A: [1..2] sync int = 1;

  A += 1; /// warning
}
test13();

proc test14() {
  writeln("test14");
  var A: [1..2] sync int = 1;

  f(A); /// warning
}
test14();

proc test15() {
  writeln("test15");
  var s$: sync bool = true;

  if s$ then /// warning
    writeln("foo");
  else
    writeln("bar");
}
test15();


// make sure that the below patterns continue to function as
// they presented challenge during deprecation.
class C {
  var s: sync int;
}
var myC = new C();

record R {
  var s: sync int;
}
var myR = new R();
