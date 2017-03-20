use SharedObject;

class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C x=", x);
  }
}

proc shareOwnershipAgain(in arg:Shared(C)) {
  writeln("in shareOwnershipAgain with arg=", arg.borrow());
}


proc shareOwnership(in arg:Shared(C)) {
  writeln("in shareOwnership with arg=", arg.borrow());
  shareOwnershipAgain(arg);
  writeln("in shareOwnership, now arg=", arg.borrow());
}

proc make() {
  var x = new Shared(new C(1));
  return x;
}

proc foo() {
  var x = make();

  shareOwnership(x);
}


foo();

