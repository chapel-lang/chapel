var x = "not it";

class MyClass {
  var x: int;

  proc type f() {
    writeln("in f ", this:string);
  }
}


proc type MyClass.g() {
  writeln("in g ", this:string);
}

writeln("MyClass.f()");
MyClass.f();
writeln("MyClass.g()");
MyClass.g();
writeln();

writeln("(owned MyClass).f()");
(owned MyClass).f();
writeln("(owned MyClass).g()");
(owned MyClass).g();
writeln();

writeln("(shared MyClass).f()");
(shared MyClass).f();
writeln("(shared MyClass).g()");
(shared MyClass).g();
writeln();

writeln("(unmanaged MyClass).f()");
(unmanaged MyClass).f();
writeln("(unmanaged MyClass).g()");
(unmanaged MyClass).g();
writeln();

writeln("(borrowed MyClass).f()");
(borrowed MyClass).f();
writeln("(borrowed MyClass).g()");
(borrowed MyClass).g();
writeln();

//writeln("(MyClass?).f()");
//(MyClass?).f();
//writeln("(MyClass?).g()");
//(MyClass?).g();
//writeln();

writeln("(owned MyClass?).f()");
(owned MyClass?).f();
writeln("(owned MyClass?).g()");
(owned MyClass?).g();
writeln();

writeln("(shared MyClass?).f()");
(shared MyClass?).f();
writeln("(shared MyClass?).g()");
(shared MyClass?).g();
writeln();

writeln("(unmanaged MyClass?).f()");
(unmanaged MyClass?).f();
writeln("(unmanaged MyClass?).g()");
(unmanaged MyClass?).g();
writeln();

writeln("(borrowed MyClass?).f()");
(borrowed MyClass?).f();
writeln("(borrowed MyClass?).g()");
(borrowed MyClass?).g();
writeln();
