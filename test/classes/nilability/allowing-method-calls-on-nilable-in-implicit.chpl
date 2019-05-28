class MyClass { var x: int; }
proc MyClass.mymethod() { writeln("hi", x); }

proc test() {
  var x: owned MyClass? = new owned MyClass(1);

  var b = x.borrow();
  b.mymethod();

  x.mymethod();
}

test();
