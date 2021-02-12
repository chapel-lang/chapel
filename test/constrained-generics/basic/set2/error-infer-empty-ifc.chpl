/*
This test verifies that an empty interface, ie one with no required functions,
requires explicit implements declarations, which cannot be inferred.
*/

interface IFC(T) { }

proc cgFun(cgArg: ?Q, msg: string) where Q implements IFC {
  writeln("in cgFun ", msg);
}

{
  int implements IFC;
  cgFun(5, "#1"); // ok
  compilerWarning("success #1");
}

{
  cgFun(5, "#2"); // error: 'int implements IFC' is not satisfied
  compilerWarning("should not be here #2");
}
