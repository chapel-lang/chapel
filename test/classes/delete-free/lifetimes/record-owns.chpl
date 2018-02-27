pragma "safe"
module recordOwns {

use OwnedObject;

record Rint {
  var x:int;
  proc init(x:int) {
    this.x = x;
    super.init();
  }
}

class SubClass {
  var x:int;
  var r:Rint;
  proc init(x:int, in r:Rint) {
    this.x = x;
    this.r = r;
    super.init();
  }
}

class MyClass {
  var sub:Owned(SubClass);
  proc init(in sub:Owned(SubClass)) {
    this.sub = sub;
    super.init();
  }
}

record RMyClass {
  var c:Owned(MyClass);
  proc init() {
    this.c = new Owned(nil:MyClass);
    super.init();
  }
  proc init(in c:Owned(MyClass)) {
    this.c = c;
    super.init();
  }
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
