// Test control flow properties of nil checking

class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

config const maybe = true;
config const n = 0;

proc badNilBothBranches() {
  var x:owned MyClass? = new owned MyClass(1);
  if maybe then
    x = nil;
  else
    x = nil;
  x!.method();
}
badNilBothBranches();

proc badNilAccessInBranch() {
  var x:owned MyClass?;
  if maybe then
    x!.method();
}
badNilAccessInBranch();

proc badNilInWhileLoop() {
  var x:owned MyClass? = new owned MyClass(1);
  while true {
    x = nil;
    x!.method();
  }
}
badNilInWhileLoop();

proc badNilInForLoop() {
  var x:owned MyClass? = new owned MyClass(1);
  for i in 1..n {
    x = nil;
    x!.method();
  }
}
badNilInForLoop();

proc badNilInCoforallLoop() {
  coforall i in 1..n {
    var x:owned MyClass?;
    x!.method();
  }
}
badNilInCoforallLoop();

proc okNilWhileLoop() {
  var x:owned MyClass?;
  while true {
    // because this block has an in-edge from
    // itself (with x being the result of new) and also from
    // above (with x being nil), the analysis decides it can't
    // say if x is nil or not.
    x!.method();
    x = new owned MyClass(1);
  }
}
okNilWhileLoop();

proc okNilWhileLoop2() {
  var x:owned MyClass?;
  var go = maybe;
  while go {
    x = new owned MyClass(1);
    go = false;
  }
  // loop might run >0 times -> no error
  x!.method();
}
okNilWhileLoop2();


proc okNilForLoop() {
  var x:owned MyClass? = new owned MyClass(1);
  for i in 1..n {
    x = nil;
  }
  // Loop might run 0 times -> no error
  x!.method();
}
okNilForLoop();

proc okNilForLoop2() {
  var x:owned MyClass?;
  for i in 1..n {
    x = new owned MyClass(1);
  }
  // Loop might run 0 times or many times -> no error
  x!.method();
}
okNilForLoop2();


proc okNilDependsOnRuntime() {
  var x:owned MyClass?;
  if maybe {
    x = new owned MyClass(1);
  }
  x!.method();
}
okNilDependsOnRuntime();
