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
proc foo(type t:Parent) {
  writeln("Parent");
}
proc foo(type t:GenericParent) {
  writeln("GenericParent");
}
proc foo(type t:GenericChild(int)) {
  writeln("GenericChild(int)");
}
proc foo(type t:object) {
  writeln("object");
}
writeln("foo");
foo(int);
foo(int(8));
foo(real);
foo(R(complex));
foo(Parent);
foo(Child);
foo(MyClass);
foo(GenericParent(int));
foo(GenericChild(int));
foo(GenericChild(real));
writeln();

proc bar(type t) where t:int {
  writeln("int");
}
proc bar(type t) where (t:integral && !t:int) {
  writeln("integral");
}
proc bar(type t) where !(t:int || t:integral || t:R || t:Parent ||
                         t:GenericParent || t:object) {
  writeln("any type");
}
proc bar(type t) where t:R {
  writeln("R");
}
proc bar(type t) where t:Parent {
  writeln("Parent");
}
proc bar(type t) where (t:GenericParent && !t:GenericChild(int)) {
  writeln("GenericParent");
}
proc bar(type t) where t:GenericChild(int) {
  writeln("GenericChild(int)");
}
proc bar(type t) where (t:object && !t:Parent && !t:GenericParent) {
  writeln("object");
}

writeln("bar");
bar(int);
bar(int(8));
bar(real);
bar(R(complex));
bar(Parent);
bar(Child);
bar(MyClass);
bar(GenericParent(int));
bar(GenericChild(int));
bar(GenericChild(real));
