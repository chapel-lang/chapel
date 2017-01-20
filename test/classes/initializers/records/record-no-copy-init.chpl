record R {
  var x:int;
  proc init() {
    writeln("in R's default-init");
  }
  // no copy init
}

proc makeR() {
  return new R();
}


proc foo() {
  var x = new R(); // no copy
  writeln(x);

  var y = makeR(); // no copy
  writeln(y);
}


foo();

