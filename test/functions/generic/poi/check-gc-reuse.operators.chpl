// Ensure generic-instantiations cache entries for '=' and '<'
// are not reused when reuse would be incorrect.
//
// This version uses compilerWarnings.

module Lib {
  record MyRecord { proc init=(other) { } }
  var myR = new MyRecord();
  proc note(param message, param depth) {
    compilerWarning(message, depth);
    writeln(message);
  }

  // these invoke '=' and '<' through POI
  proc libFun(myArg) {
    var myRec = new MyRecord();
    myRec = myArg;           // '='
    var lt = myRec < myArg;  // '<'
  }
}

module User {
  use Lib;
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("User.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("User.<", 2); return true; }

  proc u1() {
    note("User.u1", 1);
    libFun(myR);      // create a cache entry
  }
  proc u2() {
    {
      note("User.u2", 1);
      libFun(myR);    // reuse the cache entry
    }
  }
  proc u3() {
    proc =(ref lhs:MyRecord, rhs:MyRecord) { note("User.u3.=", 2); }
    proc <(lhs:MyRecord,     rhs:MyRecord) { note("User.u3.<", 2); return true; }
    note("User.u3", 1);
    libFun(myR);      // cannot reuse the cache entry -> create a new one
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
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("More1.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("More1.<", 2); return true; }

  proc m1con() {
    note("More1.m1con", 1);
    libFun(myR);      // cannot reuse the cache entry -> create a new one
  }
  proc m1gen(param p) {
    note("More1.m1gen", 1);
    libFun(myR);      // can reuse the cache entry created for m1con
  }
}

module More2 {
  use Lib;

  proc m2con() {
    note("More2.m2con", 1);
    // libFun(myR);   // error: no visible = or <
  }
  proc m2gen(param p) {
    note("More2.m2gen", 1);
    libFun(myR);      // can reuse the cache entry created for u1
  }
}

module Combo1 {
  use Lib;
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("Combo1.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("Combo1.<", 2); return true; }

  proc combo1a() {
    use Combo2;
    combo2a(0);
  }
  proc combo1b(param p) {
    note("Combo1.combo1b", 1);
    libFun(myR);    // can reuse the cache entry created for combo1a->combo2a
  }
}

module Combo2 {
  use Lib;

  proc combo2a(param p) {
    note("Combo2.combo2a", 1);
    libFun(myR);      // create a fresh cache entry
  }
  proc combo2b() {
    use Combo1;
    combo1b(0);
  }
}
