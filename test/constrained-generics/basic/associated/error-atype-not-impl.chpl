// This is associated-type-query.chpl
// where the associated type is not implemented.
// cgFun() is removed because its presence should not affect checking.

interface IFC {
  type AT;
  proc Self.reqFun(formal: AT);
  proc Self.dfltImpl(formal: AT) {
    writeln("in IFC.dfltImpl");
  }
}

record R1 {
  type AQ;
  var x1: AQ;
  proc reqFun(arg: AQ) {
    writeln("R1.reqFun(", arg, ": ", AQ:string, ")");
  }
}

implements IFC(R1(int));  // error: 'AT' is not implemented
