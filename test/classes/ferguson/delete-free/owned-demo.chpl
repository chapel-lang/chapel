// a class we will manage
class C {
  var x:int;
}

proc examples() {

  writeln("declaring owned1 storing C(1)"); 
  // 'owned1' has a unique pointer to a new C instance
  var owned1 = new owned C(1)?;
  writeln("owned1 = ", owned1.borrow());

  writeln("declaring owned2 storing nil");
  // it's OK to have owned initially empty. It will point to 'nil'.
  var owned2: owned C?;
  writeln("owned2 = ", owned2.borrow());

  // Assignment is supported, but it empties the RHS
  //
  // 'owned2' will acquire the unique pointer to what 'owned1' had (C(1)).
  // 'owned1' will contain nil
  owned2 = owned1;
  writeln("after assigning owned2 = owned1");
  writeln("owned2 = ", owned2.borrow());
  writeln("owned1 = ", owned1.borrow());

  writeln("declaring owned3 storing C(3)");
  var owned3 = new owned C(3)?;
  writeln("owned3 = ", owned3.borrow());

  // An owned variable can be re-used. Here, owned3 is filled
  // and the assignment statement passes its ownership to owned1.
  // After this line,
  //  'owned1' will contain C(3)
  //  'owned3' will contain nil
  owned1 = owned3;
  writeln("after assigning owned1 = owned3");
  writeln("owned1 = ", owned1.borrow());
  writeln("owned3 = ", owned3.borrow());

  writeln("declaring owned4 storing C(4)");
  var owned4 = new owned C(4)?;
  writeln("owned4 = ", owned4.borrow());

  // An non-empty owned variable can also be pointed to
  // something else. The value it stored before will be destructed.
  // After this line,
  //  'owned1' will contain C(4); previous object C(3) will be destroyed
  //  'owned4' will contain nil
  owned1 = owned4;
  writeln("after assigning owned1 = owned4");
  writeln("owned1 = ", owned1.borrow());
  writeln("owned4 = ", owned4.borrow());

  writeln("declaring owned5 storing C(5)");
  var owned5 = new owned C(5)?;
  writeln("owned5 = ", owned5.borrow());

  // The clear method can be used to delete the
  // managed object, if any.
  owned5.clear(); // deletes C(5)
  writeln("after owned5.clear()");
  writeln("owned5 = ", owned5.borrow());

  // The retain method can be used to provide a new value for
  // an owned to point to.
  // After this line:
  //  'owned1' will contain C(100); previous object C(4) will be destroyed
  owned1.retain(new unmanaged C(100));
  writeln("after owned1.retain C(100)");
  writeln("owned1 = ", owned1.borrow());

  // The release method can be used to extract the
  // owned pointer when something else will have responsibility
  // for freeing it. Whatever owned has release() called on it
  // will contain nil after the call.
  var ptr = owned1.release();
  writeln("after owned1.release()");
  writeln("owned1 = ", owned1.borrow());
  writeln("owned1.release() returned = ", ptr);
  delete ptr;

  // The `in` intent can be used to pass ownership from a call
  // site to the called function.
  proc takesOwnership(in arg: owned C?) {
   writeln("in takesOwnership, arg = ", arg.borrow());
    // arg's contained pointer will be deleted at the end of this function
  }

  writeln("declaring owned6 storing C(6)");
  var owned6 = new owned C(6)?;
  takesOwnership(owned6);
  // After that call, owned6 contains nil
   writeln("owned6 = ", owned6.borrow());
}

examples();
