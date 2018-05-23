class C {
  proc param foo {
    writeln("In param foo");
  }

  proc param bar() {
    writeln("In param bar()");
  }

  proc foo {
    writeln("In non-param foo");
  }

  proc bar() {
    writeln("In non-param bar()");
  }
}

var myC = new borrowed C();

myC.foo;
myC.bar();



// we don't have any way to create param classes (or records)
// currently, so we only test the non-param case.
