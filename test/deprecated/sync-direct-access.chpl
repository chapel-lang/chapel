// The operations marked with /// should result in a warning
// (or, in the future, an error).

proc test1() {
  writeln("test2");
  var b$: sync int = 1;
  var c$: sync int = 1;

  var a = b$ + c$; ///

  writeln("a = ", a, " : ", a.type:string);
}
test1();

proc test2() {
  writeln("test2");
  var x$: sync int = 1;

  x$ += 2; ///

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

  f(x$); ///

  x$.writeEF(1);
  writeln("x$ = ", x$.readFF(), " : ", x$.type:string);
}
test3();

proc test4() {
  writeln("test4");
  var x$: sync int = 1;

  var b = x$; ///

  writeln("b = ", b, " : ", b.type:string);
}
test4();

proc test5() {
  writeln("test5");
  var a$: sync int = 1;
  a$.readFE();
  var b$: sync int = 1;

  a$ = b$; ///

  b$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test5();

proc test6() {
  writeln("test6");
  var a$: sync int = 1;

  var b = a$; ///

  a$ = 1;
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test6();

proc test7() {
  writeln("test7");
  var a$: sync int = 1;

  var b$: sync int = a$; ///

  a$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b$ = ", b$.readFF(), " : ", b$.type:string);
}
test7();

proc test8() {
  writeln("test8");
  var a$: sync int = 1;

  var b: int = a$; ///

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

  var b: int = a$; ///

  a$.writeEF(1);
  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
  writeln("b = ", b, " : ", b.type:string);
}
test9();

proc test10() {
  writeln("test10");
  var a$: sync int = 1;
  a$.readFE();

  a$ = 1; ///

  writeln("a$ = ", a$.readFF(), " : ", a$.type:string);
}
test10();

proc test11() {
  writeln("test11");
  var a$: sync int = 1;

  a$; ///
}
test11();
