class C {  }

record Test1 {
  type t;
  var field: t?;
}

proc t1() {
  writeln("t1");
  var tst = new Test1(shared C);
  writeln(tst);
  writeln(tst.t:string);
  writeln(tst.field.type:string);
}
t1();

record Test2 {
  type t;
  var field: t? = nil;
}

proc t2() {
  writeln("t2");
  var tst = new Test2(shared C);
  writeln(tst);
  writeln(tst.t:string);
  writeln(tst.field.type:string);
}
t2();

record Test3 {
  type t;
  var field: t? = nil: t?;
}

proc t3() {
  writeln("t3");
  var tst = new Test3(shared C);
  writeln(tst);
  writeln(tst.t:string);
  writeln(tst.field.type:string);
}
t3();
