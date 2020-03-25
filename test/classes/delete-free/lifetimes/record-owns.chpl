pragma "safe"
module recordOwns {


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
  var sub:owned SubClass;
  proc init(in sub:owned SubClass) {
    this.sub = sub;
  }
}

record RMyClass {
  var c:owned MyClass?;
  proc init() {
    this.c = nil;
  }
  proc init(in c:owned MyClass) {
    this.c = c;
  }
  proc init=(ref other: RMyClass) {
    this.c = other.c;
  }
}

proc =(ref lhs:RMyClass, ref rhs:RMyClass) {
  lhs.c = rhs.c;
}

// Globals
var globalMyClass:owned MyClass?;
var globalRMyClass:RMyClass;

// Test initialization block
{
  var ri = new Rint(1);
  var sub = new owned SubClass(1, ri);
  globalMyClass = new owned MyClass(sub);
}

proc refIdentity(ref x) ref {
  return x;
}

proc setGlobalRecord() {
  var ri = new Rint(1);
  var sub = new owned SubClass(1, ri);
  var c = new owned MyClass(sub);
  var r = new RMyClass(c);
  
  globalRMyClass = r;
}

proc test() {
  setGlobalRecord();
}
test();

}
