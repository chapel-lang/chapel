class MyClass {
  var x;
}
class SubClass {
  var z;
}

proc qrr(arg: MyClass(?t)) {
  writeln(t:string);
}

proc grr(arg: SubClass(?t)) {
  writeln(t:string);
}

proc sub(arg: MyClass(SubClass(?t))) {
  writeln(t:string);
}

qrr(new borrowed MyClass(new borrowed SubClass(1)));
grr(new borrowed SubClass(1));
sub(new borrowed MyClass(new borrowed SubClass(2)));
