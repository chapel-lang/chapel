// Ensure that errors are issued properly.

class MyClass {
  var x:int;
}


record R1 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init() {
  }
}

record R4 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    this.complete();
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

class C1 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init() {
  }
}

class C4 {
  var fo:owned MyClass;
  var fs:shared MyClass;
  proc init(a:unmanaged MyClass, b:unmanaged MyClass) {
    this.complete();
    fo = owned.create(a);
    fs = shared.create(b);
  }
}

proc test1() {
  var r = new R1();
  var cc = new unmanaged C1();
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
test4();


record R5 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
}

record R6 {
  var fo:owned MyClass;
  var fs:shared MyClass;
}

class C5 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
}

class C6 {
  var fo:owned MyClass;
  var fs:shared MyClass;
}

// TODO: this test should also issue errors
proc test5a() {
  var r = new R5();
  var cc = new unmanaged C5();
  delete cc;
}
proc test6a() {
  var r = new R6();
  var cc = new unmanaged C6();
  delete cc;
}

test5a();
test6a();


var xxx: (int, owned MyClass);
var yyy: (int, (owned MyClass, int));
var zzz: (int, ((owned MyClass, real), real));
