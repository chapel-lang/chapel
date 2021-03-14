// This is associated-type-query.chpl
// plus it adds a constraint on the associated type.

interface IFC {
  type AT;
  AT implements Helper;
  proc Self.reqFun(formal: AT);
  proc Self.dfltImpl(formal: AT) {
    writeln("in IFC.dfltImpl");
  }
}

interface Helper { }

proc cgFun(cgArg: ?Q, atArg: Q.AT) where Q implements IFC {
  writeln("in cgFun");
  cgArg.reqFun(atArg);
  cgArg.dfltImpl(atArg);
}

record R1 {
  type AT;          // AT is implemented with a type field
  var x1: AT;
  proc reqFun(arg: AT) {
    writeln("R1.reqFun(", arg, ": ", AT:string, ")");
  }
}

int implements Helper;
implements IFC(R1(int));

cgFun(new R1(int, 55), 66);

record R2 {
  var xx;
  proc AT type      // AT is implemented with a type method
    return xx.type;
  proc reqFun(arg: AT) {
    writeln("R2(", xx, ").reqFun(", arg, ": ", AT:string, ")");
  }
}

string implements Helper;
implements IFC(R2(string));

cgFun(new R2("hello"), "world");
