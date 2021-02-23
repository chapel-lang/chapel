/*
This code tests these features:
* An interface function has a non-void return type.
* The return type of an interface function is 'Self'.
* The default implementation of an interface function
  invokes other interface functions.
* Test the above when the interface function has
  a default implementation or not.

The following test has a simpler version of this,
where default implementations do not invoke other required functions:
 returning-self.chpl
*/

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
  // These used to have inferred return types. This is no longer allowed,
  // so these do not differ from dfltSelf/dfltInt significantly.
  // They are left around to avoid editing the .good file.
  proc implicitSelf(formal5: Self): Self {
    writeln("in implicitSelf");
    return dfltSelf(reqSelf(formal5));
  }
  proc implicitInt(formal6: Self): int {
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
