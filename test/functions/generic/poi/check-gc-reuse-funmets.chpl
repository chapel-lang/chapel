// Ensure generic-instantiations cache entries for 'libFun' and 'libMet'
// are not reused when reuse would be incorrect.




module Lib {
  class MyClass { }
  var myC = new MyClass();
  proc note(param message, param depth) {
    compilerWarning(message, depth);
    writeln(message);
  }

  // these invoke workFun and workMet through POI
  proc libFun(myArg) {
    workFun();
    myArg.workMet();
  }
  proc MyClass.libMet(myArg) {
    workFun();
    myArg.workMet();
  }
}

module User {
  use Lib;
  proc workFun()         { note("User.workFun", 2); }
  proc MyClass.workMet() { note("User.workMet", 2); }

  proc u1() {
    note("User.u1", 1);
    libFun(myC);      // create a cache entry
    myC.libMet(myC);  // ditto
  }
  proc u2() {
    {
      note("User.u2", 1);
      libFun(myC);    // reuse the cache entry
      myC.libMet(myC);
    }
  }
  proc u3() {
    proc workFun()         { note("User.u3.workFun", 2); }
    proc MyClass.workMet() { note("User.u3.workMet", 2); }
    note("User.u3", 1);
    libFun(myC);      // cannot reuse the cache entry -> create a new one
    myC.libMet(myC);
  }

  proc main {
    u1();
    u2();
    u3();
    {
      use More1;
      m1con();
      m1gen(0);
    }
    {
      use More2;
      m2con();
      m2gen(0);
    }
    {
      use Combo1;
      combo1a();
    }
    {
      use Combo2;
      combo2b();
    }
  }
}

module More1 {
  use Lib;
  proc workFun()         { note("More1.workFun", 2); }
  proc MyClass.workMet() { note("More1.workMet", 2); }

  proc m1con() {
    note("More1.m1con", 1);
    libFun(myC);      // cannot reuse the cache entry -> create a new one
    myC.libMet(myC);
  }
  proc m1gen(param p) {
    note("More1.m1gen", 1);
    libFun(myC);      // can reuse the cache entry created for m1con
    myC.libMet(myC);
  }
}

module More2 {
  use Lib;

  proc m2con() {
    note("More2.m2con", 1);
    // libFun(myC);   // error: no visible workFun/Met()
    // myC.libMet(myC);
  }
  proc m2gen(param p) {
    note("More2.m2gen", 1);
    libFun(myC);      // can reuse the cache entry created for u1
    myC.libMet(myC);
  }
}

module Combo1 {
  use Lib;
  proc workFun()         { note("Combo1.workFun", 2); }
  proc MyClass.workMet() { note("Combo1.workMet", 2); }

  proc combo1a() {
    use Combo2;
    combo2a(0);
  }
  proc combo1b(param p) {
    note("Combo1.combo1b", 1);
    libFun(myC);      // can reuse the cache entry created
    myC.libMet(myC);  //  for combo1a->combo2a
  }
}

module Combo2 {
  use Lib;

  proc combo2a(param p) {
    note("Combo2.combo2a", 1);
    libFun(myC);      // create a fresh cache entry
    myC.libMet(myC);
  }
  proc combo2b() {
    use Combo1;
    combo1b(0);
  }
}
