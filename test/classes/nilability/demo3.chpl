module demo {
  class C { var value: int; }

  proc getTheValue(x: C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): C? {
    return nil;
  }
  
  
  var x: C?; // ok, has nil default value
  getTheValue(x); // compile-time error because x is nilable
}
