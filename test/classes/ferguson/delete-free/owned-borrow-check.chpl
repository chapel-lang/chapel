class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

var ownMyGlobalC = new owned C(1);
var myGlobalC:borrowed C = ownMyGlobalC.borrow();
proc bar(arg:borrowed C) {
  myGlobalC = arg;
}

proc foo() {
  var x = new owned C();

  bar(x.borrow());
  // but bar saves the borrow in myGlobal! Oops!
}


proc useGlobal() {
  writeln(myGlobalC.x); // expect a use-after-free here
}

foo();

useGlobal();
