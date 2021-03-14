module demo {
  class C { var value: int; }

  proc getTheValue(x: borrowed C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): borrowed C? {
    return nil;
  }
  
  
  var x: borrowed C?; // ok, has nil default value
  getTheValue(x); // compile-time error because x is nilable
}
