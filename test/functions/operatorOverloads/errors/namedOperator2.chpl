record R {
  operator foo() {
    writeln("In operator foo()");
  }
}

var myR: R;

myR.foo();
