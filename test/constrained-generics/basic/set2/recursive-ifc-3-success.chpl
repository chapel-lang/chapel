// This code elaborates on recursive-ifc-1-error.chpl
// by defining THREE mutually-recursive interfaces.
//
// Within the recursive-ifc-3-* family,
// it checks whether we can implement them properly.

interface Ifc1 {
  type AT1;
  AT1 implements Ifc2;
  proc reqFun1(formal1: Self): AT1;
}

interface Ifc2 {
  type AT2;
  AT2 implements Ifc3;
  proc reqFun2(formal2: Self): AT2;
}

interface Ifc3 {
  type AT3;
  AT3 implements Ifc1;
  proc reqFun3(formal3: Self);
}

implements Ifc1(int(8));
implements Ifc2(int(16));
implements Ifc3(int(32));

proc (int(8) ).AT1 type return int(16);
proc (int(32)).AT2 type return int(32);
proc (int(64)).AT3 type return int(8);

proc reqFun1(arg1: int(8) ): int(16) {
  writeln("reqFun1 ", arg1);
  return arg1*2;
}
proc reqFun2(arg2: int(16)): int(32) {
  writeln("reqFun2 ", arg2);
  return arg2*3;
}
proc reqFun3(arg3: int(32)) {
  writeln("reqFun3 ", arg3);
}

proc cgFun1(cgArg1: Ifc1) {
  writeln("in cgFun");
  reqFun1(cgArg1);
}

cgFun1(5:int(8));
