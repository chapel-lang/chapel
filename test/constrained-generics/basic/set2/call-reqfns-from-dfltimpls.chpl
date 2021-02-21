
interface IFC {
  proc reqSelf(formal1: Self): Self;
  proc reqInt(formal2: Self): int;
  proc dfltSelf(formal3: Self): Self {
    writeln("in dfltSelf");
    return reqSelf(reqSelf(formal3));
  }
  proc dfltInt(formal4: Self): int {
    writeln("in dfltInt");
    return reqInt(dfltSelf(formal4)) * 1000;
  }
  proc implicitSelf(formal5: Self) {
    writeln("in implicitSelf");
    return dfltSelf(reqSelf(formal5));
  }
  proc implicitInt(formal6: Self) {
    writeln("in implicitInt");
    return dfltInt(implicitSelf(formal6)) * 1000;
  }
}

proc cgFunSelf(cgArg1: IFC): cgArg1.type {
  writeln("in cgFunSelf");
  writeln();
  return reqSelf(dfltSelf(implicitSelf(cgArg1)));
}

proc cgFunInt(cgArg2: IFC): int {
  writeln();
  writeln("in cgFunInt");
  writeln();
  var i1 = reqInt(dfltSelf(cgArg2));
  writeln();
  var i2 = dfltInt(implicitSelf(cgArg2));
  writeln();
  var i3 = implicitInt(reqSelf(cgArg2));
  writeln();
  return i1 + i2 + i3;
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
