use OwnedObject;
use SharedObject;

class MyClass {
  var x:int;
}

record R5 {
  var fo:Owned(MyClass) = new Owned(nil:MyClass);
  var fs:Shared(MyClass) = new Shared(nil:MyClass);
}
record R6 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
}

class C5 {
  var fo:Owned(MyClass) = new Owned(nil:MyClass);
  var fs:Shared(MyClass) = new Shared(nil:MyClass);
}
class C6 {
  var fo:Owned(MyClass);
  var fs:Shared(MyClass);
}

proc test5a() {
  var r = new R5();
  var cc = new C5();
  delete cc;
}
proc test5b() {
  var a = new MyClass(1);
  var b = new MyClass(2);
  var c = new MyClass(3);
  var d = new MyClass(4);
  var r = new R5(new Owned(a), new Shared(b));
  var cc = new C5(new Owned(a), new Shared(b));
  delete cc;
}
proc test6a() {
  var r = new R6();
  var cc = new C6();
  delete cc;
}
proc test6b() {
  var a = new MyClass(1);
  var b = new MyClass(2);
  var c = new MyClass(3);
  var d = new MyClass(4);
  var r = new R6(new Owned(a), new Shared(b));
  var cc = new C6(new Owned(a), new Shared(b));
  delete cc;
}

test5a();
test5b();
test6a();
test6b();
