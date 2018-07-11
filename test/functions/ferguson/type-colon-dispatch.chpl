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

class GenericParent {
  var y;
}

class GenericChild : GenericParent {
  var z:int;
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
proc foo(type t:borrowed Parent) {
  writeln("Parent");
}
proc foo(type t:borrowed GenericParent) {
  writeln("GenericParent");
}
proc foo(type t:borrowed GenericChild(int)) {
  writeln("GenericChild(int)");
}
proc foo(type t:borrowed object) {
  writeln("object");
}
writeln("foo");
foo(int);
foo(int(8));
foo(real);
foo(R(complex));
foo(borrowed Parent);
foo(borrowed Child);
foo(borrowed MyClass);
foo(borrowed GenericParent(int));
foo(borrowed GenericChild(int));
foo(borrowed GenericChild(real));
writeln();

proc bar(type t) where t:int {
  writeln("int");
}
proc bar(type t) where (t:integral && !t:int) {
  writeln("integral");
}
proc bar(type t) where !(t:int || t:integral || t:R || t:borrowed Parent ||
                         t:borrowed GenericParent || t:borrowed object) {
  writeln("any type");
}
proc bar(type t) where t:R {
  writeln("R");
}
proc bar(type t) where t:borrowed Parent {
  writeln("Parent");
}
proc bar(type t) where (t:borrowed GenericParent && !t:borrowed GenericChild(int)) {
  writeln("GenericParent");
}
proc bar(type t) where t:borrowed GenericChild(int) {
  writeln("GenericChild(int)");
}
proc bar(type t) where (t:borrowed object && !t:borrowed Parent && !t:borrowed GenericParent) {
  writeln("object");
}

writeln("bar");
bar(int);
bar(int(8));
bar(real);
bar(R(complex));
bar(borrowed Parent);
bar(borrowed Child);
bar(borrowed MyClass);
bar(borrowed GenericParent(int));
bar(borrowed GenericChild(int));
bar(borrowed GenericChild(real));
