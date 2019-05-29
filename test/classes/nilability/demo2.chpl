module demo {
  class C { var value: int; }

  proc getTheValue(x: C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): C? {
    return nil;
  }

  var a: C; // error, MyClass can't be nil/has no default value!
}
