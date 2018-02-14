pragma "safe"
module l13 {

class MyClass {
  var x:int;
}

class MyOtherClass {
  var c:MyClass;
}


// It's OK to return a ref to a class field
proc getX(c:MyClass) ref {
  return c.x;
}

proc getC(cc:MyOtherClass) ref {
  return cc.c;
}

proc test() {
  var c = new MyClass(1);
  var cc = new MyOtherClass(c);

  ref rx = getX(c);
  ref rc = getC(cc);

  rx = 2;
  rc = new MyClass(3);

  writeln(c);
  writeln(cc);
}

test();
}
