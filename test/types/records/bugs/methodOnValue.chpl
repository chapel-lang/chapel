record R {
}

var myR: R;

// this is OK, since R is a type
proc R.foo() {
  writeln("In foo()");
}

// this shouldn't be OK, myR is a value
proc myR.bar() {
  writeln("In bar()");
}

myR.foo();
myR.bar();
