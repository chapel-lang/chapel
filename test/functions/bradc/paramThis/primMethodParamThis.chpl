class C {
  def param foo {
    writeln("In param foo");
  }
  def param bar() {
    writeln("In param bar()");
  }
  def foo {
    writeln("In non-param foo");
  }
  def bar() {
    writeln("In non-param bar()");
  }
}

var myC = new C();
myC.foo;
myC.bar();

// we don't have any way to create param classes (or records)
// currently, so we only test the non-param case.
