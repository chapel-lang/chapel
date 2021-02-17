/*
This code tests the following features:
* declared return type of interface functions
 - Self type
 - non-interface type
* constrained-generic function invoking such functions
* default implementations returning Self-typed values
* Self-typed values get passed around
*/

interface IFC {
  proc reqSelf(formal1: Self): Self;
  proc reqInt(formal2: Self): int;
  proc dfltSelf(formal3: Self): Self {
    writeln("in dfltSelf");
    return formal3;
  }
  proc dfltInt(formal4: Self): int {
    writeln("in dfltInt");
    return 22;
  }
  proc implicitSelf(formal5: Self) {
    writeln("in implicitSelf");
    return formal5;
  }
  proc implicitInt(formal6: Self) {
    writeln("in implicitInt");
    return 33;
  }
}

proc cgFunSelf(cgArg1: IFC): cgArg1.type {
  writeln("in cgFunSelf");
  return reqSelf(dfltSelf(implicitSelf(cgArg1)));
}

proc cgFunInt(cgArg2: IFC): int {
  writeln("in cgFunInt");
  var i1 = reqInt(dfltSelf(cgArg2));
  var i2 = dfltInt(implicitSelf(cgArg2));
  var i3 = implicitInt(reqSelf(cgArg2));
  return i1*1000000 + i2*1000 + i3;
}

implements IFC(string);

proc reqSelf(arg1: string): string {
  writeln("reqSelf(", arg1, ")");
  return arg1 + "x";
}

proc reqInt(arg2: string): int {
  writeln("reqInt(", arg2, ")");
  return arg2.size;
}

writeln(cgFunInt(cgFunSelf("world")));
