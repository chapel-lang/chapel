/*
This test checks that the compiler distinguishes
between associated types of the same interface
in generic implements statements.
*/

interface IFC1 {
  type AT1;
  type AT2;
  type AT3;
  type AT4;
  proc reqFun(cgFormal: Self, atFormal1: AT1, atFormal2: AT2,
                              atFormal3: AT3, atFormal4: AT4);
}

record MyRec {
  type AT1;
  type AT2;
  type AT3;
  type AT4;
}

proc reqFun(cgFormal:  MyRec,
            atFormal1: cgFormal.AT1,
            atFormal2: cgFormal.AT3, // should be AT2
            atFormal3: cgFormal.AT2, // should be AT3
            atFormal4: cgFormal.AT4): void
{
  writeln("in reqFun(MyRec1)");
}

implements IFC1(MyRec(int, real, real, string)); // ok
MyRec implements IFC1; // error: reqFun implementation misfits IFC1.reqFun
compilerError("done");
