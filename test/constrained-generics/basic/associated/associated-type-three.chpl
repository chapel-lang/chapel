// This is associated-type-query.chpl
// with three associated types.

interface IFC {
  type AT1;
  type AT2;
  type AT3;

  proc reqFunS(formal: Self);
  proc reqFun123(formal1: AT1, formal2: AT2, formal3: AT3);
  proc Self.reqMeth1(formal1: AT1);
  proc Self.reqMeth23(formal2: AT2, formal3: AT3);

  proc dfltFunS(formal: Self)
  {  writeln("IFC.dfltFunS");    }
  proc dfltFun123(formal1: AT1, formal2: AT2, formal3: AT3)
  {  writeln("IFC.dfltFun123");  }
  proc Self.dfltMeth1(formal1: AT1)
  {  writeln("IFC.dfltMeth1");   }
  proc Self.dfltMeth23(formal2: AT2, formal3: AT3)
  {  writeln("IFC.dfltMeth23");  }
}

proc cgFun(cgArg: ?Q, arg1: Q.AT3, arg2: Q.AT2, arg3: cgArg.AT1)
    where Q implements IFC
{
  writeln("in cgFun#1");
}

proc cgFun(cgArg: ?Q, arg1: Q.AT1, arg2: Q.AT2, arg3: cgArg.AT3)
    where Q implements IFC
{
  writeln("in cgFun#2");

  reqFunS(cgArg);
  reqFun123(arg1, arg2, arg3);
  cgArg.reqMeth1(arg1);
  cgArg.reqMeth23(arg2, arg3);

  dfltFunS(cgArg);
  dfltFun123(arg1, arg2, arg3);
  cgArg.dfltMeth1(arg1);
  cgArg.dfltMeth23(arg2, arg3);
}

record RR {
  type AT1;
  proc AT2 type return AT1;
  proc AT3 type return bool;
  var xx: AT1;

  proc reqMeth1(formal1: AT1) {
    writeln("RR(", xx, "): ", this.type:string, ").reqMeth1(",
            formal1, ": ", formal1.type:string, ")");
  }
}

proc RR.reqMeth23(formal2: int, formal3: AT3) {
  writeln("RR(", xx, "): ", this.type:string, ").reqMeth23(",
          formal2, ", ", formal3, ": ", formal3.type:string, ")");
}

proc reqFunS(formal: RR) {
  writeln("reqFunS(", formal, ": ", formal.type:string, ")");
}
proc reqFun123(formal1: int, formal2, formal3: bool) {
  writeln("reqFun123(", formal1, ", ", formal2, ", ", formal3, ")");
}

implements IFC(RR(int));

// should call cgFun#2
cgFun(new RR(int, 55), 66, 77, true);
