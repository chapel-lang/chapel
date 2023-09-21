use CTypes;

// not exported, C knows nothing about Foo
class Foo {
  var x: int;
  proc getX() const : int {
    return x;
  }
}

export proc getInstance(out addrOut: c_ptr(void), arg: int) {
  // unmanaged so it will survive the end of scope
  var myFoo = new unmanaged Foo(arg:int);
  addrOut = c_ptrTo(myFoo);
}

export proc useInstance(instancePtr: c_ptr(void)) : int {
  // have to cast to nilable, but we expect it won't be nil
  var myReconstitutedFoo = (instancePtr:unmanaged Foo?)!;
  return myReconstitutedFoo.getX();
}

export proc deleteInstance(instancePtr: c_ptr(void)) {
  var myReconstitutedFoo = (instancePtr:unmanaged Foo?)!;
  delete myReconstitutedFoo;
  // any reference to the heap instance is now invalid
}

proc main() {
  var someFoo : c_ptr(void);
  getInstance(someFoo, 5);
  writeln(useInstance(someFoo));
  deleteInstance(someFoo);
}
