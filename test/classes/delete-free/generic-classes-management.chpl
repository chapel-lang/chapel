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

 
  // accepts non-nilable borrowed any-class
  proc acceptsBorrowedNonNilable( arg: borrowed class ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts nilable borrowed any-class
  proc acceptsBorrowedNilable( arg: borrowed class? ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts any-nilable borrowed any-class
  proc acceptsBorrowedAny( arg: borrowed ) {
    writeln(arg.type:string, " ", arg);
  }


  // accepts non-nilable unmanaged any-class
  proc acceptsUnmanagedNonNilable( in arg: unmanaged class ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts nilable unmanaged any-class
  proc acceptsUnmanagedNilable( in arg: unmanaged class? ) {
    writeln(arg.type:string, " ", arg);
  }

  // accepts any-nilable unmanaged any-class
  proc acceptsUnmanagedAny( in arg: unmanaged ) {
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

    var myOwnedNilable:owned MyClass? = new owned MyClass();
    var mySharedNonNilable:shared MyClass = new shared MyClass();
    var mySharedNilable:shared MyClass? = new shared MyClass();
    var myBorrowedNonNilable:borrowed MyClass = (new owned MyClass()).borrow();
    var myBorrowedNilable:borrowed MyClass? = (new owned MyClass()).borrow();
    var myUnmanagedNonNilable:unmanaged MyClass = myBorrowedNonNilable:unmanaged;
    var myUnmanagedNilable:unmanaged MyClass? = myBorrowedNilable:unmanaged;
    var myInt = 1;

    {
      param fnName = "acceptsOwnedNonNilable";
      writeln(fnName);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsOwnedNonNilable(myOwnedNonNilable);
      }
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsOwnedNilable(myOwnedNonNilable);
      }
      acceptsOwnedNilable(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsOwnedAny(myOwnedNonNilable);
      }
      acceptsOwnedAny(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
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
      param fnName = "acceptsBorrowedNonNilable";
      writeln(fnName);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsBorrowedNonNilable(myOwnedNonNilable);
      }
      acceptsBorrowedNonNilable(mySharedNonNilable);
      acceptsBorrowedNonNilable(myBorrowedNonNilable);
      acceptsBorrowedNonNilable(myUnmanagedNonNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
      param fnName = "acceptsBorrowedNilable";
      writeln(fnName);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsBorrowedNilable(myOwnedNonNilable);
      }
      acceptsBorrowedNilable(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      acceptsBorrowedNilable(mySharedNonNilable);
      acceptsBorrowedNilable(mySharedNilable);
      acceptsBorrowedNilable(myBorrowedNonNilable);
      acceptsBorrowedNilable(myBorrowedNilable);
      acceptsBorrowedNilable(myUnmanagedNonNilable);
      acceptsBorrowedNilable(myUnmanagedNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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

    {
      param fnName = "acceptsBorrowedAny";
      writeln(fnName);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsBorrowedAny(myOwnedNonNilable);
      }
      acceptsBorrowedAny(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      acceptsBorrowedAny(mySharedNonNilable);
      acceptsBorrowedAny(mySharedNilable);
      acceptsBorrowedAny(myBorrowedNonNilable);
      acceptsBorrowedAny(myBorrowedNilable);
      acceptsBorrowedAny(myUnmanagedNonNilable);
      acceptsBorrowedAny(myUnmanagedNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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

    {
      param fnName = "acceptsUnmanagedNonNilable";
      writeln(fnName);
      acceptsUnmanagedNonNilable(myUnmanagedNonNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(canResolve(fnName, myUnmanagedNonNilable));
      assert(!canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsUnmanagedNilable";
      writeln(fnName);
      acceptsUnmanagedNilable(myUnmanagedNonNilable);
      acceptsUnmanagedNilable(myUnmanagedNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(canResolve(fnName, myUnmanagedNonNilable));
      assert(canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }

    {
      param fnName = "acceptsUnmanagedAny";
      writeln(fnName);
      acceptsUnmanagedAny(myUnmanagedNonNilable);
      acceptsUnmanagedAny(myUnmanagedNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(!canResolve(fnName, myOwnedNonNilable));
      }
      assert(!canResolve(fnName, myOwnedNilable));
      myOwnedNilable = new owned MyClass();
      assert(!canResolve(fnName, mySharedNonNilable));
      assert(!canResolve(fnName, mySharedNilable));
      assert(!canResolve(fnName, myBorrowedNonNilable));
      assert(!canResolve(fnName, myBorrowedNilable));
      assert(canResolve(fnName, myUnmanagedNonNilable));
      assert(canResolve(fnName, myUnmanagedNilable));
      assert(!canResolve(fnName, myInt));
    }


    {
      param fnName = "acceptsClassAnyManagementNonNilable";
      writeln(fnName);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsClassAnyManagementNonNilable(myOwnedNonNilable);
      }
      acceptsClassAnyManagementNonNilable(mySharedNonNilable);
      acceptsClassAnyManagementNonNilable(myBorrowedNonNilable);
      acceptsClassAnyManagementNonNilable(myUnmanagedNonNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        acceptsClassAnyManagementNilable(myOwnedNonNilable);
      }
      acceptsClassAnyManagementNilable(myOwnedNilable);
      myOwnedNilable = new owned MyClass();
      acceptsClassAnyManagementNilable(mySharedNonNilable);
      acceptsClassAnyManagementNilable(mySharedNilable);
      acceptsClassAnyManagementNilable(myBorrowedNonNilable);
      acceptsClassAnyManagementNilable(myBorrowedNilable);
      acceptsClassAnyManagementNilable(myUnmanagedNonNilable);
      acceptsClassAnyManagementNilable(myUnmanagedNilable);
      {
        var myOwnedNonNilable:owned MyClass = new owned MyClass();
        assert(canResolve(fnName, myOwnedNonNilable));
      }
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
