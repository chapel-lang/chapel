module demo {
  class C { var value: int; }

  proc getTheValue(x: C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): C? {
    return nil;
  }
  getTheValue(nil); // compile-time error
}
