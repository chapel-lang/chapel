class MyClass {
  var x;
}

class OtherClass {
  var y;
}

class Concrete {
  var z:int;
}

proc g(arg:unmanaged MyClass(?t)) {
  writeln("g ", t:string);
}
proc h(arg:unmanaged MyClass(unmanaged OtherClass(?t))) {
  writeln("h ", t:string);
}

proc i(arg:unmanaged) {
  writeln("i ", arg.type:string);
}
proc j(arg:unmanaged MyClass) {
  writeln("j ", arg.type:string);
}
proc k(arg:unmanaged MyClass(unmanaged OtherClass)) {
  writeln("k ", arg.type:string);
}

proc ll(arg:unmanaged MyClass) {
  writeln("ll unmanaged MyClass");
}
proc ll(arg:borrowed MyClass) {
  writeln("ll borrowed MyClass");
}
proc ll(arg) {
  writeln("ll generic");
}

proc m(type t : unmanaged MyClass) {
  writeln("m unmanaged MyClass");
}
proc m(type t : borrowed MyClass) {
  writeln("m borrowed MyClass");
}
proc m(type t : unmanaged object) {
  writeln("m unmanaged object");
}
proc m(type t : borrowed object) {
  writeln("m borrowed object");
}

proc n(arg : unmanaged MyClass) {
  writeln("n unmanaged MyClass");
}
proc n(arg : unmanaged object) {
  writeln("n unmanaged object");
}
proc n(arg : borrowed MyClass) {
  writeln("n borrowed MyClass");
}


proc test() {
  var a = new unmanaged MyClass(new unmanaged OtherClass(1));
  var b = new unmanaged Concrete();
  g(a);
  h(a);
  i(a);
  j(a);
  k(a);
  ll(a);
  ll(_to_borrowed(a));
  ll(b);
  m(unmanaged MyClass(int));
  m(unmanaged Concrete);
  m(borrowed MyClass(int));
  m(borrowed Concrete);
  n(a);
  n(_to_borrowed(a));
  n(b);
  delete b;
  delete a.x;
  delete a;
}

test();
