

proc foo(x) where x:integral {
  writeln("integral");
}
proc foo(x) {
  writeln("any type");
}

writeln("foo");
var i8:int(8) = 1;
foo(i8);
writeln();

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
writeln();

proc test1() where GenericParent(int):GenericChild { writeln("BAD"); }
proc test1() { writeln("test1"); }
proc test2() where GenericChild(real):GenericParent { writeln("test2"); }
proc test2() { writeln("BAD"); }
proc test3() where GenericChild(int):GenericParent { writeln("test3"); }
proc test3() { writeln("BAD"); }
proc test4() where GenericChild(int):GenericParent(int) { writeln("test4"); }
proc test4() { writeln("BAD"); }
proc test5() where GenericChild(real):GenericParent(int) { writeln("BAD"); }
proc test5() { writeln("test5"); }
proc test6() where GenericChild(int(8)):GenericParent(int) { writeln("BAD"); }
proc test6() { writeln("test6"); }

proc test7() where int:integral { writeln("test7"); }
proc test7() { writeln("BAD"); }
proc test8() where real:integral { writeln("BAD"); }
proc test8() { writeln("test8"); }
proc test9() where real:numeric { writeln("test9"); }
proc test9() { writeln("BAD"); }

test1();
test2();
test3();
test4();
test5();
test6();
test7();
test8();
test9();
