module demo {
  class C { var value: int; }

  proc getTheValue(x: borrowed C) {
   return x.value; // no check needed here
  }
  proc someComplexFunctionReturningNil(): borrowed C? {
    return nil;
  }

  





  getTheValue(someComplexFunctionReturningNil() ! ); // adds rt check
}
