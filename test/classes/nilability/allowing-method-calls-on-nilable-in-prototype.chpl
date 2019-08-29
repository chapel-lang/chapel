prototype module mymod {
  class MyClass { var x: int; }
  proc MyClass.mymethod() { writeln("hi", x); }

  proc test1() {
    var x: owned MyClass? = new owned MyClass(1);

    var b = x.borrow();
    b.mymethod();

    x.mymethod();
  }
  test1();

  class GenericClass {
    var x;
    proc mymethod() {
      writeln("hi", x);
    }
  }

  proc test2() {
    var c:borrowed GenericClass? = new GenericClass(2);

    var b = c.borrow();
    b.mymethod();

    var x = c.x;
    writeln(x);
  }
  test2();
}
