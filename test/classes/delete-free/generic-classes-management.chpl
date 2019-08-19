module testmodule {

  // Work-around for const-ref version only in Reflection
  // causing 'coercion passed to const ref' errors
  proc canResolve(param fname : string, in arg) param : bool
    return __primitive("call resolves", fname, arg);


  class MyClass { var x:int = 1; }

  // accepts non-nilable owned any-class
  proc acceptsOwnedNonNilable( in arg: owned class ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts nilable owned any-class
  proc acceptsOwnedNilable( in arg: owned class? ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts any-nilable owned any-class
  proc acceptsOwnedAny( in arg: owned ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts non-nilable shared any-class
  proc acceptsSharedNonNilable( in arg: shared class ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts nilable shared any-class
  proc acceptsSharedNilable( in arg: shared class? ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts any-nilable shared any-class
  proc acceptsSharedAny( in arg: shared ) {
    writeln(arg.type:string, " ", arg);
  }

 
  // accepts any-management non-nilable any-class
  proc acceptsClassAnyManagementNonNilable( in arg: class ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts any-management nilable any-class
  proc acceptsClassAnyManagementNilable( in arg: class? ) {
    writeln(arg.type:string, " ", arg);
  }

  proc main() {

    var myOwnedNonNilable:owned MyClass = new owned MyClass();
    var myOwnedNilable:owned MyClass? = new owned MyClass();
    var mySharedNonNilable:shared MyClass = new shared MyClass();
    var mySharedNilable:shared MyClass? = new shared MyClass();
    var myBorrowedNonNilable:borrowed MyClass = myOwnedNonNilable.borrow();
    var myBorrowedNilable:borrowed MyClass? = myOwnedNilable.borrow();
    var myUnmanagedNonNilable:unmanaged MyClass = myBorrowedNonNilable:unmanaged;
    var myUnmanagedNilable:unmanaged MyClass? = myBorrowedNilable:unmanaged;
    var myInt = 1;

    {
      param fnName = "acceptsOwnedNonNilable";
      writeln(fnName);
      acceptsOwnedNonNilable(myOwnedNonNilable);
      myOwnedNonNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }
    
    {
      param fnName = "acceptsOwnedNilable";
      writeln(fnName);
      acceptsOwnedNilable(myOwnedNonNilable);
      myOwnedNonNilable = new owned MyClass();
      acceptsOwnedNilable(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsOwnedAny";
      writeln(fnName);
      acceptsOwnedAny(myOwnedNonNilable);
      myOwnedNonNilable = new owned MyClass();
      acceptsOwnedAny(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsSharedNonNilable";
      writeln(fnName);
      acceptsSharedNonNilable(mySharedNonNilable);
      assert(!canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsSharedNilable";
      writeln(fnName);
      acceptsSharedNilable(mySharedNonNilable);
      acceptsSharedNilable(mySharedNilable);
      assert(!canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, mySharedNonNilable));
      assert(canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsSharedAny";
      writeln(fnName);
      acceptsSharedAny(mySharedNonNilable);
      acceptsSharedAny(mySharedNilable);
      assert(!canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, mySharedNonNilable));
      assert(canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(!canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsClassAnyManagementNonNilable";
      writeln(fnName);
      acceptsClassAnyManagementNonNilable(myOwnedNonNilable);
      myOwnedNonNilable = new owned MyClass();
      acceptsClassAnyManagementNonNilable(mySharedNonNilable);
      acceptsClassAnyManagementNonNilable(myBorrowedNonNilable);
      acceptsClassAnyManagementNonNilable(myUnmanagedNonNilable);
      assert(canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsClassAnyManagementNilable";
      writeln(fnName);
      acceptsClassAnyManagementNilable(myOwnedNonNilable);
      myOwnedNonNilable = new owned MyClass();
      acceptsClassAnyManagementNilable(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      acceptsClassAnyManagementNilable(mySharedNonNilable);
      acceptsClassAnyManagementNilable(mySharedNilable);
      acceptsClassAnyManagementNilable(myBorrowedNonNilable);
      acceptsClassAnyManagementNilable(myBorrowedNilable);
      acceptsClassAnyManagementNilable(myUnmanagedNonNilable);
      acceptsClassAnyManagementNilable(myUnmanagedNilable);
      assert(canResolve(fnName, myOwnedNonNilable));
      myOwnedNonNilable = new owned MyClass();
      assert(canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(canResolve(fnName, mySharedNonNilable));
      assert(canResolve(fnName, mySharedNilable));
      assert(canResolve(fnName, myBorrowedNonNilable));
      assert(canResolve(fnName, myBorrowedNilable));
      assert(canResolve(fnName, myUnmanagedNonNilable));
      assert(canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }
  }
}
