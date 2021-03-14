module demo {
  class C { var value: int; }

  proc getTheValue(x: borrowed C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): borrowed C? {
    return nil;
  }

  
  var x: borrowed C?; // ok, has nil default value

  getTheValue(x ! ); // x! adds a run-time check
                     // (unless the compiler can prove to skip it)
}
