// The operations marked with /// should result in a warning
// (or, in the future, an error).

proc test1() {
  writeln("test2");
  var b$: single int = 1;
  var c$: single int = 1;

  var a = b$ + c$; ///  warning

  writeln("a = ", a, " : ", a.type:string);
}
test1();

// cannot += a single

// not tested here:
//  proc f(in arg)
//  proc f(in arg:int)
// because this results in backend compiler errors before deprecation

proc f(arg: int) {
  writeln("in f arg = ", arg, " : ", arg.type:string);
}
proc test3() {
  writeln("test3");
  var x$: single int = 1;

  f(x$); /// warning

  writeln("x$ = ", x$.readFF(), " : ", x$.type:string);
}
test3();

proc test4() {
  writeln("test4");
  var x$: single int = 1;

  var b = x$; /// warning

  writeln("b = ", b, " : ", b.type:string);
}
test4();

proc test5() {
  writeln("test5");
  var a$: single int;
  a$.isFull;
  var b$: single int = 1;
  b$.isFull;

  a$ = b$; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test5();

proc test6() {
  writeln("test6");
  var a$: single int = 1;

  var b = a$; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test6();

proc test7() {
  writeln("test7");
  var a$: single int = 1;

  var b$: single int = a$; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test7();

proc test8() {
  writeln("test8");
  var a$: single int = 1;

  var b: int = a$; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test8();

proc test9() {
  writeln("test9");
  var a$: single int = 1;
  a$.readFF();

  var b: int = a$; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test9();

proc test10() {
  writeln("test10");
  var a$: single int;
  a$.isFull;

  a$ = 1; /// warning

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
}
test10();

proc test11() {
  writeln("test11");
  var a$: single int = 1;

  a$; /// warning
}
test11();

proc test12() {
  writeln("test12");
  var A: [1..2] single int;
  writeln(A.domain); // to avoid split init

  A = 1; /// warning
}
test12();

// A += 1 does not make sense for single

proc test14() {
  writeln("test14");
  var A: [1..2] single int = 1;

  f(A); /// warning
}
test14();

proc test15() {
  writeln("test15");
  var s$: single bool = true;

  if s$ then /// warning
    writeln("foo");
  else
    writeln("bar");
}
test15();


// make sure that the below patterns continue to function as
// they presented challenge during deprecation.
class C {
  var s: single int;
}
var myC = new C();

record R {
  var s: single int;
}
var myR = new R();
myR.s.writeEF(1);
var tmpR = myR; // reads myR.s and stores it in the new one
var otherR = new R();
otherR.s.isFull;
otherR = tmpR; // reads tmpR.s and stores it in myR
writeln(otherR.s.readFF());
