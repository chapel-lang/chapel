// An extern types should be the same regardless of where it comes from.

module withExternType {

  // a generic function - can have multiple instantiations
  proc makeAB(type tt) type {
    extern type atomic_bool;
    return atomic_bool;
  }

  // a concrete function
  proc makeAB() type {
    extern type atomic_bool;
    return atomic_bool;
  }

  extern type atomic_bool;

  proc callAB(arg: makeAB(int)) {
    compilerError("success");
  }

  module aModule {
    extern type atomic_bool;
  }

  proc main {
    import this.aModule;
    type t1 = makeAB(int);
    type t2 = makeAB(int); // another call to the same instantiation
    type t3 = makeAB(bool);
    type t4 = makeAB();
    type t5 = aModule.atomic_bool;
    type t6 = atomic_bool;
    compilerWarning("t1==t2 ", (t1==t2):string);
    compilerWarning("t1==t3 ", (t1==t3):string);
    compilerWarning("t1==t4 ", (t1==t4):string);
    compilerWarning("t1==t5 ", (t1==t5):string);
    compilerWarning("t1==t6 ", (t1==t5):string);
    compilerWarning("t2==t3 ", (t2==t3):string);
    compilerWarning("t2==t4 ", (t2==t4):string);
    compilerWarning("t2==t5 ", (t2==t5):string);
    compilerWarning("t2==t6 ", (t2==t5):string);
    compilerWarning("t3==t4 ", (t3==t4):string);
    compilerWarning("t3==t5 ", (t3==t5):string);
    compilerWarning("t3==t6 ", (t3==t5):string);
    compilerWarning("t4==t5 ", (t4==t5):string);
    compilerWarning("t4==t6 ", (t4==t5):string);
    compilerWarning("t5==t6 ", (t4==t5):string);

    var x: makeAB(bool);
    callAB(x);
  }

}  // module withExternType
