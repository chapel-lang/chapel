use OwnedObject;

class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

var myGlobalC:C;
proc bar(arg:C) {
  myGlobalC = arg;
}

proc foo() {
  var x = new Owned(new C());

  bar(x.borrow());
  // but bar saves the borrow in myGlobal! Oops!
}


proc useGlobal() {
  writeln(myGlobalC.x); // expect a use-after-free here
}

foo();

useGlobal();
