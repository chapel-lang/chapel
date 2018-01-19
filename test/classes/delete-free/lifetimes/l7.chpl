use OwnedObject;

class MyClass {
  var x:int;
}
record SubR {
  var c:MyClass;
}
record R {
  var sub:SubR;
}

proc buildR(sub:SubR) {
  return new R(sub);
}

var globalR:R;

proc test() {
  var c = new Owned(new MyClass(1));
  var subr = new SubR(c.borrow());
  globalR = buildR(subr);
}

test();
