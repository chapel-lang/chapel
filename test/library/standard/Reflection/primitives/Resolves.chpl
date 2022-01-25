//
// Check the behavior of PRIM_RESOLVES, a primitive that can be used to
// implement functions in the Reflection module.
//

proc compilerAssert(param expr: bool) param {
  if !expr then compilerError('Assert failed!', 1);
}

proc doProcedureZero() { writeln('zero'); }
proc doProcedureOne(a) { writeln(a); }
proc doProcedureTwo(a, b) { writeln(a, " ", b); }
proc doProcedureParam(param a) { writeln(a); }
proc doProcedureType(type t) { writeln(t); }

proc test1() {
  param yes = __primitive("resolves", doProcedureZero());
  compilerAssert(yes);
}
test1();

proc test2() {
  param yes1 = __primitive("resolves", doProcedureOne(0));
  compilerAssert(yes1);
  param yes2 = __primitive("resolves", doProcedureOne(a=0));
  compilerAssert(yes2);
  var x = 0;
  param yes3 = __primitive("resolves", doProcedureOne(x));
  compilerAssert(yes3);
  param yes4 = __primitive("resolves", doProcedureOne(a=x));
  compilerAssert(yes4);
  ref y = x;
  param yes5 = __primitive("resolves", doProcedureOne(y));
  compilerAssert(yes5);
  param yes6 = __primitive("resolves", doProcedureOne(a=y));
  compilerAssert(yes6);
  const ref z = x;
  param yes7 = __primitive("resolves", doProcedureOne(z));
  compilerAssert(yes7);
  param yes8 = __primitive("resolves", doProcedureOne(a=z));
  compilerAssert(yes8);
  param no1 = __primitive("resolves", doProcedureOne(foo=z));
  compilerAssert(!no1);
  type T = r;
  param no2 = __primitive("resolves", doProcedureOne(T));
  compilerAssert(!no2);
}
test2();

proc test3() {
  param yes1 = __primitive("resolves", doProcedureTwo(0, new r()));
  compilerAssert(yes1);
}
test3();

proc test4() {
  param val = 8;
  param yes1 = __primitive("resolves", doProcedureParam(val));
  compilerAssert(yes1);
}
test4();

proc test5() {
  type T = (int, real);
  param yes1 = __primitive("resolves", doProcedureType(T));
  compilerAssert(yes1);
}
test5();

record r {
  proc doMethodZero() { writeln('zero'); }
  proc doMethodOne(a) { writeln(a); }
  proc doMethodTwo(a, b) {writeln(a, " ", b); }
  proc doMethodParam(param val) param { return val + 1; }
  proc type doMethodType() { writeln('type'); }
}

proc test6() {
  var obj = new r();
  param yes1 = __primitive("resolves", obj.doMethodZero());
  compilerAssert(yes1);

}
test6();

proc test7() {
  var obj = new r();
  param yes1 = __primitive("resolves", obj.doMethodOne(a='foo'));
  compilerAssert(yes1);
}
test7();

proc test8() {
  var obj = new r();
  param val = 0.0;
  param yes1 = __primitive("resolves", obj.doMethodTwo(a=val, b=new r()));
  compilerAssert(yes1);
}
test8();

proc test9() {
  var obj = new r();
  param val = 0.0;
  param yes1 = __primitive("resolves", obj.doMethodParam(val=val));
  compilerAssert(yes1);
}
test9();

proc test10() {
  param yes1 = __primitive("resolves", r.doMethodType());
  compilerAssert(yes1);
}
test10();

