class MyClass {
  var x:int;
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc test1() {
  writeln("test1");
  var a: unmanaged MyClass = new unmanaged MyClass();
  // 'a' refers to a manually managed instance that needs to be 'delete'd at some point
  var b: owned MyClass = new owned MyClass();
  // the instance referred to by 'b' is deleted at end of scope
  var c: shared MyClass = new shared MyClass();
  // the instance referred to by 'c' is reference counted
  var d: borrowed MyClass = new borrowed MyClass();
  // the instance referred to by 'd' will be deleted at the end of scope

  writeln(a.type:string);
  writeln(b.type:string);
  writeln(c.type:string);
  writeln(d.type:string);

  delete a;
}
test1();

proc test2() {
  writeln("test2");
  var a                    = new unmanaged MyClass();
  // 'a' refers to a manually managed instance that needs to be 'delete'd at some point
  var b                = new owned MyClass();
  // the instance referred to by 'b' is deleted at end of scope
  var c                 = new shared MyClass();
  // the instance referred to by 'c' is reference counted
  var d                   = new borrowed MyClass();
  // the instance referred to by 'd' will be deleted at the end of scope

  writeln(a.type:string);
  writeln(b.type:string);
  writeln(c.type:string);
  writeln(d.type:string);

  delete a;
}
test2();
