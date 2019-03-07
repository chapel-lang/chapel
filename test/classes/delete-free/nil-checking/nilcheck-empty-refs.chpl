class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

proc badRefToNilOwned() {
  var x: owned MyClass;
  ref r = x;
  r.method();
}
badRefToNilOwned();

proc badSetNilByRef() {
  var x: owned MyClass = new owned MyClass(1);
  ref r = x;
  r = nil;
  x.method();
}
badSetNilByRef();

proc badSetNilByRefX() {
  var x: owned MyClass = new owned MyClass(1);
  ref r = x;
  x = nil;
  r.method();
}
badSetNilByRefX();

proc badSetNilByRefAssign() {
  var x: owned MyClass = new owned MyClass(1);
  var y: owned MyClass;
  ref r = x;
  y = r; // clears x/r
  x.method();
}
badSetNilByRefAssign();

proc badSetNilByRefXAssign() {
  var x: owned MyClass = new owned MyClass(1);
  var y: owned MyClass;
  ref r = x;
  y = x; // clears x/r
  r.method();
}
badSetNilByRefXAssign();
