class Foo {
  var i1    : volatile int;
  var i2    : volatile int = 2;

  var ui1   : volatile uint;
  var ui2   : volatile uint = 2;

  var r1    : volatile real;
  var r2    : volatile real = 2.0;

  def m1() {
    write(i1,  ", ");
    write(i2,  ", ");
    write(ui1, ", ");
    write(ui2, ", ");
    write(r1,  ", ");
    writeln(r2);
  }
}
def main() {
  const f1 = new Foo();
  const f2 = new Foo(1, 2, 3, 4, 5.0, 6.0);

  f1.m1();
  f2.m1();
}
