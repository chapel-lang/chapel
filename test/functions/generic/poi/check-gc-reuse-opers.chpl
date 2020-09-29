// Ensure generic-instantiations cache entries for libFun(1),
// which use type operations, are not reused when reuse would be incorrect.
//
// This is an inverse of check-gc-reuse-inits.chpl -- here the
// initializers and operators are called from libFun() through its POIs
// so it is the cache entries for the latter to be reused correctly.


module Lib {
  record MyRecord { param rp; proc deinit() {} }

  proc note(param message, param depth) {
    compilerWarning(message, depth);
    if depth <= 1 then writeln();
    writeln(message);
  }

  // this invokes init, init=, =, <, _cast through POI
  proc libFun(myArg) {
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
}

module User {
  use Lib;
  proc MyRecord.init(param p)            { note("User.init", 2); rp = 0; }
  proc MyRecord.init=(other)             { note("User.init=", 2); rp = 0; }
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("User.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("User.<", 2);  return true; }
  proc _cast(type t:int,   rhs:MyRecord) { note("User._castF", 2); return 1; }
  proc _cast(type t:MyRecord,   rhs:int) { note("User._castT", 2);
                                           return new MyRecord(0); }
  proc u1() {
    note("User.u1", 1);
    libFun(1);      // create a cache entry
  }
  proc u2() {
    {
      note("User.u2", 1);
      libFun(1);    // reuse the cache entry
    }
  }
  proc u3() {
    proc MyRecord.init(param p)            { note("u3.init", 2); rp = 0; }
    proc MyRecord.init=(other)             { note("u3.init=", 2); rp = 0; }
    proc =(ref lhs:MyRecord, rhs:MyRecord) { note("u3.=", 2); }
    proc <(lhs:MyRecord,     rhs:MyRecord) { note("u3.<", 2); return true; }
    proc _cast(type t:int,   rhs:MyRecord) { note("u3._castF", 2); return 1; }
    proc _cast(type t:MyRecord,   rhs:int) { note("u3._castT", 2);
                                             return new MyRecord(0); }
    note("User.u3", 1);
    libFun(1);      // cannot reuse the cache entry -> create a new one
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
    writeln();
  }
}

module More1 {
  use Lib;
  proc MyRecord.init(param p)            { note("More1.init", 2); rp = 0; }
  proc MyRecord.init=(other)             { note("More1.init=", 2); rp = 0; }
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("More1.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("More1.<", 2); return true; }
  proc _cast(type t:int,   rhs:MyRecord) { note("More1._castF", 2); return 1; }
  proc _cast(type t:MyRecord,   rhs:int) { note("More1._castT", 2);
                                           return new MyRecord(0); }
  proc m1con() {
    note("More1.m1con", 1);
    libFun(1);      // cannot reuse the cache entry -> create a new one
  }
  proc m1gen(param p) {
    note("More1.m1gen", 1);
    libFun(1);      // can reuse the cache entry created for m1con
  }
}

module More2 {
  use Lib;

  proc m2con() {
    note("More2.m2con", 1);
    // libFun(1);   // error: no visible = or <
  }
  proc m2gen(param p) {
    note("More2.m2gen", 1);
    libFun(1);      // can reuse the cache entry created for u1
  }
}

module Combo1 {
  use Lib;
  proc MyRecord.init(param p)            { note("Combo1.init", 2); rp = 0; }
  proc MyRecord.init=(other)             { note("Combo1.init=", 2); rp = 0; }
  proc =(ref lhs:MyRecord, rhs:MyRecord) { note("Combo1.=", 2); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { note("Combo1.<", 2); return true; }
  proc _cast(type t:int,   rhs:MyRecord) { note("Combo1._castF", 2); return 1; }
  proc _cast(type t:MyRecord,   rhs:int) { note("Combo1._castT", 2);
                                           return new MyRecord(0); }
  proc combo1a() {
    use Combo2;
    combo2a(0);
  }
  proc combo1b(param p) {
    note("Combo1.combo1b", 1);
    libFun(1);      // can reuse the cache entry created for combo1a->combo2a
  }
}

module Combo2 {
  use Lib;

  proc combo2a(param p) {
    note("Combo2.combo2a", 1);
    libFun(1);      // create a fresh cache entry
  }
  proc combo2b() {
    use Combo1;
    combo1b(0);
  }
}
