pragma "safe"
module l5 {



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
  var sub:borrowed SubClass;
  proc init(sub:borrowed SubClass) {
    this.sub = sub;
  }
}

record RMyClass {
  var c:borrowed MyClass?;
  proc init() {
    this.c = nil;
  }
  proc init(c:borrowed MyClass) {
    this.c = c;
  }
}

// Globals
var globalMyClass:borrowed MyClass?;
var globalRMyClass:RMyClass;

// Test initialization block
{
  var ri = new Rint(1);
  var sub = new unmanaged SubClass(1, ri);
  globalMyClass = new unmanaged MyClass(sub);
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
  var sub = new unmanaged SubClass(1, ri);
  var c = new unmanaged MyClass(sub);
  var r = new RMyClass(c);

  globalRMyClass = r;
}

class MyClassA {
  var x:int;
}
record SubRA {
  var c:borrowed MyClassA?;
}
record RA {
  var sub:SubRA;
}

proc buildR(sub:SubRA) {
  return new RA(sub);
}

var globalRA:RA;

proc bad() {
  var c = new owned MyClassA(1);
  var subr = new SubRA(c.borrow());
  globalRA = buildR(subr);
}


proc test() {
  ref x = badReturnRefLocalRec();
  writeln(x);

  setGlobalRecord();
  bad();
}
test();

}
