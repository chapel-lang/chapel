// This test checks several patterns of
// records and classes with owned and shared fields.
// This test is intended to make sure that fields
// of owned or shared type continue to function.

class MyClass {
  var x:int;
}

record R1 {
  var fo:owned MyClass?;
  var fs:shared MyClass?;
  proc init() {
  }
}

record R2 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
  proc init() {
  }
}

record R3 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

record R4 {
  var fo:owned MyClass?;
  var fs:shared MyClass?;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    this.complete();
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

class C1 {
  var fo:owned MyClass?;
  var fs:shared MyClass?;
  proc init() {
  }
}

class C2 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
  proc init() {
  }
}

class C3 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

class C4 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    super.init();
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

proc test1() {
  var r = new R1();
  var cc = new unmanaged C1();
  delete cc;
}
proc test2() {
  var r = new R2();
  var cc = new unmanaged C2();
  delete cc;
}
proc test3() {
  var a = new unmanaged MyClass(1);
  var b = new unmanaged MyClass(2);
  var c = new unmanaged MyClass(3);
  var d = new unmanaged MyClass(4);
  var r = new R3(a,b);
  var cc = new unmanaged C3(c,d);
  delete cc;
}
proc test4() {
  var a = new unmanaged MyClass(1);
  var b = new unmanaged MyClass(2);
  var c = new unmanaged MyClass(3);
  var d = new unmanaged MyClass(4);
  var r = new R4(a,b);
  var cc = new unmanaged C4(c,d);
  delete cc;
}


test1();
test2();
test3();
test4();
