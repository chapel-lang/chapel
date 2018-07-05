pragma "use default init"
class MyClass {
  var x;
}

pragma "use default init"
class OtherClass {
  var y;
}

proc g(arg:borrowed MyClass(?t)) {
  writeln(t:string);
}
proc h(arg:borrowed MyClass(borrowed OtherClass(?t))) {
  writeln(t:string);
}

proc i(arg:borrowed) {
  writeln(arg.type:string);
}
proc j(arg:borrowed MyClass) {
  writeln(arg.type:string);
}
proc k(arg:borrowed MyClass(borrowed OtherClass)) {
  writeln(arg.type:string);
}



proc test() {
  var a = new borrowed MyClass(new borrowed OtherClass(1));
  g(a);
  h(a);
  i(a);
  j(a);
  k(a);
}

test();
