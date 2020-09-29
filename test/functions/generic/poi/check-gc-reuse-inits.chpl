// Ensure generic-instantiations cache entries for init=, =, <, _cast et al.
// are not reused when reuse would be incorrect.
//
// This is an inverse of check-gc-reuse-opers.chpl -- here it is the
// initializers and operators whose cache entries need to be reused correctly
// as they call functions through their POIs.


module Lib {
  record MyRecord { param rp; }

  proc note(param message, param depth) {
    compilerWarning(message, depth);
    if depth <= 1 then writeln();
    writeln(message);
  }

  // these invoke workIN et al. through POI
  proc MyRecord.init(param p) { workIN(); rp = 0; }
  proc MyRecord.init=(other)  { workIE(); rp = 0; }
  proc MyRecord.deinit()      { workDI(); }
  proc =(ref lhs:MyRecord, rhs:MyRecord) { workAS(); }
  proc <(lhs:MyRecord,     rhs:MyRecord) { workLT(); return true; }
  proc _cast(type t:int,   rhs:MyRecord) { workCF(); return 1; }
  proc _cast(type t:MyRecord,   rhs:int) { workCT(); return new MyRecord(0); }
}

module User {
  use Lib;
  proc workIN() { note("User.workIN", 2); }
  proc workIE() { note("User.workIE", 2); }
  proc workDI() { note("User.workDI", 2); }
  proc workAS() { note("User.workAS", 2); }
  proc workLT() { note("User.workLT", 2); }
  proc workCF() { note("User.workCF", 2); }
  proc workCT() { note("User.workCT", 2); }

  proc u1() {
    note("User.u1", 1);
    // create a cache entry
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
  proc u2() {
    {
      note("User.u2", 1);
      // reuse the cache entry
      var r1 = new MyRecord(0); // init
      var r2 = r1;              // init=
      r1 = r2;                  // =
      var lt = r1 < r2;         // <
      var castF = r1: int;      // _cast from
      var castT = 1: MyRecord;  // _cast to
    }
  }
  proc u3() {
    proc workIN() { note("u3.workIN", 2); }
    proc workIE() { note("u3.workIE", 2); }
    proc workDI() { note("u3.workDI", 2); }
    proc workAS() { note("u3.workAS", 2); }
    proc workLT() { note("u3.workLT", 2); }
    proc workCF() { note("u3.workCF", 2); }
    proc workCT() { note("u3.workCT", 2); }

    note("User.u3", 1);
    // cannot reuse the cache entry -> create a new one
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
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
  proc workIN() { note("More1.workIN", 2); }
  proc workIE() { note("More1.workIE", 2); }
  proc workDI() { note("More1.workDI", 2); }
  proc workAS() { note("More1.workAS", 2); }
  proc workLT() { note("More1.workLT", 2); }
  proc workCF() { note("More1.workCF", 2); }
  proc workCT() { note("More1.workCT", 2); }

  proc m1con() {
    note("More1.m1con", 1);
    // cannot reuse the cache entry -> create a new one
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
  proc m1gen(param p) {
    note("More1.m1gen", 1);
    // can reuse the cache entry created for m1con
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
}

module More2 {
  use Lib;

  proc m2con() {
    note("More2.m2con", 1);
    // MyRecord operations would cause error: no visible workIN et al.
  }
  proc m2gen(param p) {
    note("More2.m2gen", 1);
    // can reuse the cache entry created for u1
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
}

module Combo1 {
  use Lib;
  proc workIN() { note("Combo1.workIN", 2); }
  proc workIE() { note("Combo1.workIE", 2); }
  proc workDI() { note("Combo1.workDI", 2); }
  proc workAS() { note("Combo1.workAS", 2); }
  proc workLT() { note("Combo1.workLT", 2); }
  proc workCF() { note("Combo1.workCF", 2); }
  proc workCT() { note("Combo1.workCT", 2); }

  proc combo1a() {
    use Combo2;
    combo2a(0);
  }
  proc combo1b(param p) {
    note("Combo1.combo1b", 1);
    // can reuse the cache entry created for combo1a->combo2a
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
}

module Combo2 {
  use Lib;

  proc combo2a(param p) {
    note("Combo2.combo2a", 1);
    // create a fresh cache entry
    var r1 = new MyRecord(0); // init
    var r2 = r1;              // init=
    r1 = r2;                  // =
    var lt = r1 < r2;         // <
    var castF = r1: int;      // _cast from
    var castT = 1: MyRecord;  // _cast to
  }
  proc combo2b() {
    use Combo1;
    combo1b(0);
  }
}
