class MyClass { type T; var x : T; }

var a = new unmanaged MyClass(int, 1);
writeln(a);
delete a;
 // OK

var c = new owned MyClass(int, 1);
writeln(c);
 // OK

proc makeit(type t) {
  return new t(1);
}

var e = makeit(owned MyClass(int));
writeln(e);
