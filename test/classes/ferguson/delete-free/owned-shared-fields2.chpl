class MyClass {
  var x:int;
}

record R5 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
}

record R6 {
  var fo:owned MyClass?;
  var fs:shared MyClass?;
}

class C5 {
  var fo:owned MyClass? = nil;
  var fs:shared MyClass? = nil;
}

class C6 {
  var fo:owned MyClass?;
  var fs:shared MyClass?;
}

proc test5a() {
  var r = new R5();
  var cc = new unmanaged C5();
  delete cc;
}
proc test5b() {
  var a = new owned MyClass(1);
  var b = new shared MyClass(2);
  var c = new owned MyClass(3);
  var d = new shared MyClass(4);
  var r = new R5(a, b);
  var cc = new unmanaged C5(c, d);
  delete cc;
}
proc test6a() {
  var r = new R6();
  var cc = new unmanaged C6();
  delete cc;
}
proc test6b() {
  var a = new owned MyClass(1);
  var b = new shared MyClass(2);
  var c = new owned MyClass(3);
  var d = new shared MyClass(4);
  var r = new R6(a, b);
  var cc = new unmanaged C6(c, d);
  delete cc;
}

test5a();
test5b();
test6a();
test6b();
