// Test the cases where CG types are used to instantiate record types.

record PP {
  type tt;
  param pr;
}

record RR{
  var xx;
}

interface IFC {
  proc reqFn(formal1: RR(Self),
             formal2: PP(RR(Self), 37),
             formal3: PP(AT, 48)
             ): RR(AT);
  proc makeRR(formal1: PP(AT, 48),
              formal2: AT
              ): RR(Self);
  type AT;
  implements I2(PP(AT));
}

int implements IFC;
proc int.AT type return string;

interface I2 { }
implements I2(PP(string));

proc reqFn(formal1: RR(int),
           formal2: PP(RR(int)),
           formal3: PP(string, 48)
           ): RR(string)
{
  writeln("reqFn.int.string");
  return new RR("from reqFn.int.string");
}

proc makeRR(formal1: PP(string, 48), formal2: string): RR(int) {
  writeln("makeRR.int.string");
  return new RR(222);
}

proc cgFun(arg1: ?Q,
           arg2: PP(RR(Q), 37),
           arg3: PP(Q.AT, 48),
           arg4: Q.AT
           ): RR(arg1.AT)
  where Q implements IFC
{
  writeln("cgFun");
  const result = reqFn(makeRR(arg3, arg4), arg2, arg3);
  return result;
}

const result = cgFun(55,
                     new PP(RR(int), 37),
                     new PP(string, 48),
                     "hi");

writeln("result = ", result, " : ", result.type:string);
