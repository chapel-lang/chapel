pragma "safe"
module t8 {

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
  var sub:SubClass;
  proc init(sub:SubClass) {
    this.sub = sub;
    super.init();
  }
}

record RMyClass {
  var c:MyClass;
  proc init() {
    this.c = nil;
    super.init();
  }
  proc init(c:MyClass) {
    this.c = c;
    super.init();
  }
}

// Globals
var globalMyClass:MyClass;
var globalRMyClass:RMyClass;

// Test initialization block
{
  var ri = new Rint(1);
  var sub = new SubClass(1, ri);
  globalMyClass = new MyClass(sub);
}

proc refIdentity(ref x) ref {
  return x;
}

proc badReturnRefLocalRec() ref {
  var r = new Rint(1);
  return refIdentity(r);
}

proc setGlobalRecord() {
  var ri = new Rint(1);
  var sub = new SubClass(1, ri);
  var c = new MyClass(sub);
  var r = new RMyClass(c);
  
  globalRMyClass = r;
}

proc test() {
  ref x = badReturnRefLocalRec();
  writeln(x);

  setGlobalRecord();
}
test();

}
