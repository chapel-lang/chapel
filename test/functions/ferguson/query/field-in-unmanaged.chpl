class MyClass {
  var x;
}
proc f(arg: unmanaged MyClass(?t)) {
  writeln(t:string);
}

var unm = new unmanaged MyClass(1);
f(unm);
delete unm;
