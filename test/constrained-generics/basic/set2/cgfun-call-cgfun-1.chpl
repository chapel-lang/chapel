/*
CG function #1 invokes CG function #2
where the interface constraint of #2 is satisfied with that of #1.
*/

interface IFC {
  proc show(msg: string, arg: Self);
  proc writeln(msg: string, arg: Self);
}

int implements IFC;
proc show(msg: string, arg: int) { writeln(msg, arg); }

cgFun1(123);

proc cgFun1(arg1: ?Q1) where Q1 implements IFC {
  show("cgFun1: ", arg1);
  writeln("cgFun1: ", arg1);
  cgFun2(arg1);
}

proc cgFun2(arg2: ?Q2) where Q2 implements IFC {
  show("cgFun2: ", arg2);
  writeln("cgFun2: ", arg2);
}
