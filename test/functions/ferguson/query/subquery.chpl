class MyClass {
  var x;
}
class SubClass {
  var z;
}

proc qrr(arg: borrowed MyClass(?t)) {
  writeln(t:string);
}

proc grr(arg: borrowed SubClass(?t)) {
  writeln(t:string);
}

proc sub(arg: borrowed MyClass(borrowed SubClass(?t))) {
  writeln(t:string);
}

qrr((new owned MyClass((new owned SubClass(1)).borrow())).borrow());
grr((new owned SubClass(1)).borrow());
sub((new owned MyClass((new owned SubClass(2)).borrow())).borrow());
