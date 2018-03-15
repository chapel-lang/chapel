pragma "safe"
module recordOwns {

use OwnedObject;

record Rint {
  var x:int;
  proc init(x:int) {
    this.x = x;
  }
}

class SubClass {
  var x:int;
  var r:Rint;
  proc init(x:int, in r:Rint) {
    this.x = x;
    this.r = r;
  }
}

class MyClass {
  var sub:Owned(SubClass);
  proc init(in sub:Owned(SubClass)) {
    this.sub = sub;
  }
}

record RMyClass {
  var c:Owned(MyClass);
  proc init() {
    this.c = new Owned(nil:MyClass);
  }
  proc init(in c:Owned(MyClass)) {
    this.c = c;
  }
}

proc =(ref lhs:RMyClass, ref rhs:RMyClass) {
  lhs.c = rhs.c;
}

// Globals
var globalMyClass:Owned(MyClass);
var globalRMyClass:RMyClass;

// Test initialization block
{
  var ri = new Rint(1);
  var sub = new Owned(new SubClass(1, ri));
  globalMyClass = new Owned(new MyClass(sub));
}

proc refIdentity(ref x) ref {
  return x;
}

proc setGlobalRecord() {
  var ri = new Rint(1);
  var sub = new Owned(new SubClass(1, ri));
  var c = new Owned(new MyClass(sub));
  var r = new RMyClass(c);
  
  globalRMyClass = r;
}

proc test() {
  setGlobalRecord();
}
test();

}
