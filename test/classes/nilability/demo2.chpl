module demo {
  class C { var value: int; }

  proc getTheValue(x: borrowed C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): borrowed C? {
    return nil;
  }

  var a: borrowed C; // error, MyClass can't be nil/has no default value!
}
