class MyClass { var x:int; }

var a = new unmanaged MyClass(1);
writeln(a);
delete a;
 // OK

var c = new owned MyClass(1);
writeln(c);
 // OK

proc makeit(type t) {
  return new t(1);
}

var e = makeit(owned MyClass);
writeln(e);
