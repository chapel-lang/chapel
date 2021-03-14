pragma "safe"
module l13 {

class MyClass {
  var x:int;
}

class MyOtherClass {
  var c:borrowed MyClass;
}


// It's OK to return a ref to a class field
proc getX(c:borrowed MyClass) ref {
  return c.x;
}

proc getC(cc:borrowed MyOtherClass) ref {
  return cc.c;
}

proc test() {
  var c = new unmanaged MyClass(1);
  var cc = new unmanaged MyOtherClass(c);

  ref rx = getX(c);
  ref rc = getC(cc);

  rx = 2;
  var tc = new unmanaged MyClass(3);
  rc = tc;

  writeln(c);
  writeln(cc);

  delete tc, cc, c;
}

test();
}
