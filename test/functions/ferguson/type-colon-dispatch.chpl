record R {
  var x;
}

class Parent {
  var a:int;
}

class Child : Parent {
  var b:int;
}

class MyClass {
  var c:int;
}

proc foo(type t:int) {
  writeln("int");
}
proc foo(type t:integral) {
  writeln("integral");
}
proc foo(type t) {
  writeln("any type");
}
proc foo(type t:R) {
  writeln("R");
}
proc foo(type t:Parent) {
  writeln("Parent");
}
proc foo(type t:object) {
  writeln("object");
}


foo(int);
foo(int(8));
foo(real);
foo(R(complex));
foo(Parent);
foo(Child);
foo(MyClass);
