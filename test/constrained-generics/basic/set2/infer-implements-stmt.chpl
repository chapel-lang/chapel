/*
This test checks implicit inference of 'implements' declarations.

Test cases when a desired 'implements' declaration cannot be inferred:
  error-implements-constraint.chpl
  error-infer-empty-ifc.chpl
*/

interface IFC(T) {
  proc reqFun(cgFormal: T);
  proc dfltImpl(cgFormal: T) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, msg: string) where Q implements IFC {
  writeln("in cgFun() ", msg);
  reqFun(cgArg);
  dfltImpl(cgArg);
}

// This scope has no 'implements' declarations.
// They cannot be inferred either.


{
  // This scope has no 'implements' declarations.
  // They CAN be inferred.

  proc reqFun(arg: int) {
    writeln("in reqFun#1 ", arg);
  }

  cgFun(11, "#1");    // reqFun implementation in the current scope

  {
    cgFun(12, "#1");  // reqFun implementation in the enclosing scope
  }

  {
    proc reqFun(arg: int) {
      writeln("in reqFun#2 ", arg);
    }

    cgFun(20, "#2");  // reqFun implementation overrides
  }

  {
    proc reqFun(arg: string) {
      writeln("in reqFun#3 ", arg);
    }

    cgFun("30", "#3");  // unrelated reqFun implementation
  }

}
