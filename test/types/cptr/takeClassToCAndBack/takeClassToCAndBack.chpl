use CTypes;

// not exported, C knows nothing about Foo
class Foo {
  var x: int;
  proc getX() const : int {
    return x;
  }
}

export proc getInstance(ref addrOut: c_void_ptr, arg: int) {
  // unmanaged so it will survive the end of scope
  var myFoo = new unmanaged Foo(arg:int);
  addrOut = c_ptrTo(myFoo);
}

export proc useInstance(instancePtr: c_void_ptr) : int {
  // have to cast to nilable, but we expect it won't be nil
  var myReconstitutedFoo = (instancePtr:unmanaged Foo?)!;
  return myReconstitutedFoo.getX();
}

export proc deleteInstance(instancePtr: c_void_ptr) {
  var myReconstitutedFoo = (instancePtr:unmanaged Foo?)!;
  delete myReconstitutedFoo;
  // any reference to the heap instance is now invalid
}

