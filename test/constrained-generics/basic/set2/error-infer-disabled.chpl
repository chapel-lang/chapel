/*
This test runs a part of infer-implements-stmt.chpl
with --no-infer-implements-decls
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
}
