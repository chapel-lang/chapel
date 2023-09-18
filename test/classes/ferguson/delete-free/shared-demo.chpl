

// a class we will manage
class C {
  var x:int;
}

proc examples() {

  writeln("declaring shared1 storing C(1)"); 
  // 'shared1' points to a new C instance
  var shared1 = new shared C(1)?;
  writeln("shared1 = ", shared1.borrow());

  writeln("declaring shared2 storing nil");
  // it's OK to have shared initially empty. It will point to 'nil'.
  var shared2: shared C?;
  writeln("shared2 = ", shared2.borrow());

  // Assignment is supported. The LHS and RHS will both point
  // to the same class and share responsibility for deleting it.
  shared2 = shared1;
  writeln("after assigning shared2 = shared1");
  writeln("shared2 = ", shared2.borrow());
  writeln("shared1 = ", shared1.borrow());

  writeln("declaring shared3 storing C(3)");
  var shared3 = new shared C(3);
  writeln("shared3 = ", shared3.borrow());

  // A shared variable can be set to something else.
  // Here, shared1 is set to shared3.
  //  'shared1' will contain C(3)
  //  'shared3' will contain C(3)
  shared1 = shared3;
  writeln("after assigning shared1 = shared3");
  writeln("shared1 = ", shared1.borrow());
  writeln("shared3 = ", shared3.borrow());

  // The retain method can be used to provide a new value
  // for a shared to point to.
  // After this line:
  //  shared1 will contain C(100)
  shared1 = shared.adopt(new unmanaged C(100));
  writeln("after shared1 = shared.adopt C(100)");
  writeln("shared1 = ", shared1.borrow());

  // The clear method can be used to empty a shared immediately.
  // If the shared record is the last one referring to the object
  // it contains, that object will be deleted.
  // For shared, release does not return a value. That is because
  // it could still be managed by other shared record instances.
  shared1 = nil;
  writeln("after shared1 = nil");
  writeln("shared1 = ", shared1.borrow());

  writeln("declaring shared4 storing C(4)");
  var shared4 = new shared C(4);
  writeln("shared4 = ", shared4.borrow());

  // The `in` intent can be used to share ownership from a call
  // site to the called function.
  proc sharesOwnership(in arg: shared C) {
   writeln("in sharesOwnership, arg = ", arg.borrow());
    // arg's contained pointer will be deleted at the end of this function
  }

  writeln("declaring shared5 storing C(5)");
  var shared5 = new shared C(5);
  sharesOwnership(shared5);
  // After that call, shared5 contains nil
   writeln("shared5 = ", shared5.borrow());
}

examples();
