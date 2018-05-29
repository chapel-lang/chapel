class MyClass {
  var x:int;
  
  proc primaryMethod() {
    writeln("In MyClass.primaryMethod(), this.type is ", this.type:string);
  }
}

proc f(arg:unmanaged MyClass) {
  writeln("in f unmanaged");
}
proc f(arg:borrowed MyClass) {
  writeln("in f borrowed");
}

proc call_f(type t) {
  var something = new t();
  f(something);
}

proc MyClass.secondaryMethod() {
  writeln("In MyClass.secondaryMethod(), this.type is ", this.type:string);
}

proc test1() {
  var unm:unmanaged MyClass = new unmanaged MyClass();
  var bor:borrowed MyClass = unm;
  var def:MyClass = unm;

  writeln("Calling f(unmanaged)");
  f(unm);
  writeln("Calling f(borrowed)");
  f(bor);
  writeln("Calling f(default)");
  f(def);

  delete unm;
}
test1();

proc test2() {
  var unm:unmanaged MyClass = new unmanaged MyClass();
  var def:MyClass = unm;

  writeln("Calling call_f(unmanaged MyClass)");
  call_f(unmanaged MyClass);
  call_f(unm.type);
  writeln("Calling call_f(MyClass)");
  call_f(MyClass);
  call_f(def.type);

  delete unm;
}
test2();

proc test3() {
  var unm:unmanaged MyClass = new unmanaged MyClass();

  writeln("Calling method on unmanaged");
  unm.primaryMethod();
  unm.secondaryMethod();

  delete unm;
}
test3();

proc test4() {
  var def = new MyClass();
  writeln("Calling f(new MyClass())");
  f(def);
  f(new MyClass());
}
test4();
