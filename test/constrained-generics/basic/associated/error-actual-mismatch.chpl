// This is a part of associated-type-three.chpl
// where the actuals' types are mismatched.

interface IFC {
  type AT1;
  type AT2;
  type AT3;

  proc reqFunS(formal: Self);
}

proc cgFun(cgArg: ?Q, arg1: Q.AT3, arg2: Q.AT2, arg3: cgArg.AT1)
    where Q implements IFC
{
  writeln("in cgFun#1");
  reqFunS(cgArg);
}

record RR {
  type AT1;
  proc AT2 type return AT1;
  proc AT3 type return bool;
}

// RR implements IFC implicitly, thanks to this fn.
proc reqFunS(formal: RR) {
  writeln("reqFunS(", formal, ": ", formal.type:string, ")");
}

cgFun(new RR(int), true, 66, 77);  // ok
cgFun(new RR(int), 66, 77, true);  // error
