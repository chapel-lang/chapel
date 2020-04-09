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

proc returnRef(const ref x) {
  const ref xx = x; // work around for existing issue
  return xx; // error here if no copy initializer generated
}

proc foo() {
  var a = new R(); // no copy
  writeln(a);

  var b = makeR(); // no copy
  writeln(b);

  var c = returnRef(a); // error inside returnRef if no copy
  writeln(c);

  var d = b; // error here
  writeln(d);
}


foo();

