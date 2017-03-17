use SharedObject;

class C {
  var x:int;
  proc ~C() {
    writeln("Destroying C x=", x);
  }
}

proc takeOwnershipAgain(in arg:Shared(C)) {
  writeln("in takeOwnershipAgain with arg=", arg.borrow());
}


proc takeOwnership(in arg:Shared(C)) {
  writeln("in takeOwnership with arg=", arg.borrow());
  takeOwnershipAgain(arg);
  writeln("in takeOwnership, now arg=", arg.borrow());
}



proc foo() {
  var x = new Shared(new C(1));

  takeOwnership(x);
}


foo();

