//  lhs: borrowed!  rhs: nil?  error: nlb

class MyClass {
  var x: int;
}



var lhs: borrowed MyClass = nil;

//todo: uncomment this without changing .good:   compilerError("done");
