// This test checks several patterns of
// records and classes with Owned and Shared fields.
// This test is intended to make sure that fields
// of Owned or Shared type continue to function.

use OwnedObject;
use SharedObject;

class MyClass {
  var x:int;
}

record R1 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init() {
  }
}

record R2 {
  var fo:Owned(MyClass) = new Owned(nil:unmanaged MyClass);
  var fs:Shared(MyClass) = new Shared(nil:unmanaged MyClass);
  proc init() {
  }
}

record R3 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    fo = new Owned(a);
    fs = new Shared(b);
  }
}

record R4 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    this.complete();
    fo = new Owned(a);
    fs = new Shared(b);
  }
}

class C1 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init() {
  }
}

class C2 {
  var fo:Owned(MyClass) = new Owned(nil:unmanaged MyClass);
  var fs:Shared(MyClass) = new Shared(nil:unmanaged MyClass);
  proc init() {
  }
}

class C3 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    fo = new Owned(a);
    fs = new Shared(b);
  }
}

class C4 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    super.init();
    fo = new Owned(a);
    fs = new Shared(b);
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
