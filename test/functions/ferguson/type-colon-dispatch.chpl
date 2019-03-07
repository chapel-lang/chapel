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
foo(uint);
foo(real);
foo(R(complex));
foo(borrowed Parent);
foo(borrowed Child);
foo(borrowed MyClass);
foo(borrowed GenericParent(int));
foo(borrowed GenericChild(int));
foo(borrowed GenericChild(real));
writeln();

proc bar(type t) where isSubtype(t,int) {
  writeln("int");
}
proc bar(type t) where (isSubtype(t,integral) && !isSubtype(t,int)) {
  writeln("integral");
}
proc bar(type t) where !(isSubtype(t,int) || isSubtype(t,integral) ||
                         isSubtype(t,R) || isSubtype(t,borrowed Parent) ||
                         isSubtype(t,borrowed GenericParent) ||
                         isSubtype(t,borrowed object)) {
  writeln("any type");
}
proc bar(type t) where isSubtype(t,R) {
  writeln("R");
}
proc bar(type t) where isSubtype(t,borrowed Parent) {
  writeln("Parent");
}
proc bar(type t) where (isSubtype(t,borrowed GenericParent) &&
                        !isSubtype(t,borrowed GenericChild(int))) {
  writeln("GenericParent");
}
proc bar(type t) where isSubtype(t,borrowed GenericChild(int)) {
  writeln("GenericChild(int)");
}
proc bar(type t) where (isSubtype(t,borrowed object) &&
                        !isSubtype(t,borrowed Parent) &&
                        !isSubtype(t,borrowed GenericParent)) {
  writeln("object");
}

writeln("bar");
bar(int);
bar(int(8));
bar(uint);
bar(real);
bar(R(complex));
bar(borrowed Parent);
bar(borrowed Child);
bar(borrowed MyClass);
bar(borrowed GenericParent(int));
bar(borrowed GenericChild(int));
bar(borrowed GenericChild(real));
