// This is associated-constraint-empty.chpl
// where the associated constraint is not satisfied.
// cgFun() is removed because its presence should not affect checking.

interface IFC {
  type AT;
  AT implements Helper;
  proc Self.reqFun(formal: AT);
  proc Self.dfltImpl(formal: AT) {
    writeln("in IFC.dfltImpl");
  }
}

interface Helper { }

record R1 {
  type AT;          // AT is implemented with a type field
  var x1: AT;
  proc reqFun(arg: AT) {
    writeln("R1.reqFun(", arg, ": ", AT:string, ")");
  }
}

int implements Helper;
implements IFC(R1(int)); // ok

record R2 {
  var xx;
  proc AT type      // AT is implemented with a type method
    return xx.type;
  proc reqFun(arg: AT) {
    writeln("R2(", xx, ").reqFun(", arg, ": ", AT:string, ")");
  }
}

implements IFC(R2(string)); // error: AT=string does not implement Helper
