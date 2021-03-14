
record R {
  var x:int;
}

record RGeneric {
  var x;
}

proc test() {
  var a = new unmanaged R(1);
  var b = new unmanaged RGeneric(1);
  var c = new owned R(1);
  var d = new owned RGeneric(1);

  writeln(a);
  writeln(b);
  writeln(c);
  writeln(d);
}

test();
