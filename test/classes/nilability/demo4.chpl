module demo {
  class C { var value: int; }

  proc getTheValue(x: C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): C? {
    return nil;
  }

  
  var x: C?; // ok, has nil default value

  getTheValue(x ! ); // x! adds a run-time check
                 // (unless the compiler can prove to skip it)
  
  getTheValue(someComplexFunctionReturningNil()); // compile-time error
}
