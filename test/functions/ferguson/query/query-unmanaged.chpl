pragma "use default init"
class MyClass {
  var x;
}

pragma "use default init"
class OtherClass {
  var y;
}

pragma "use default init"
class Concrete {
  var z:int;
}

proc g(arg:unmanaged MyClass(?t)) {
  writeln(t:string);
}
proc h(arg:unmanaged MyClass(unmanaged OtherClass(?t))) {
  writeln(t:string);
}

proc i(arg:unmanaged) {
  writeln(arg.type:string);
}
proc j(arg:unmanaged MyClass) {
  writeln(arg.type:string);
}
proc k(arg:unmanaged MyClass(unmanaged OtherClass)) {
  writeln(arg.type:string);
}

proc ll(arg:unmanaged MyClass) {
  writeln(arg.type:string);
}
proc ll(arg:borrowed MyClass) {
  writeln(arg.type:string);
}
proc ll(arg) {
  writeln(arg.type:string);
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
  ll(_to_borowed(a));
  ll(b);
  delete b;
  delete a.x;
  delete a;
}

test();
