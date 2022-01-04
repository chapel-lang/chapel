// This code contains a "recursive" interface, i.e.
// its associated type is constrained by the same interface.
//
// This code tests whether we can successfully implement such an interface.

interface IFC {
  type AT;
  AT implements IFC;
  proc reqFun(formal: Self);
}

int implements IFC;
proc int.AT type return int;
proc reqFun(arg: int) {
  writeln("reqFun ", arg);
}

proc cgFun(cgArg: IFC) {
  writeln("in cgFun");
  reqFun(cgArg);
}

cgFun(5);
