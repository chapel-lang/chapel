// Ensure generic-instantiations cache entries for 'libfn'
// are not reused when reuse would be incorrect.
//
// This version uses compilerWarnings.

module Work {

module Lib {
  proc libfn(param p) {
    worker();  // invoke through POI
  }
}

module User {
  use super.Lib;
  proc worker() { compilerWarning("User.worker", 1); }

  proc u1() {
    libfn(0);  // create a cache entry
  }

  proc u2() {
    {
      libfn(0);  // reuse the cache entry
    }
  }

  proc u3() {
    proc worker() { compilerWarning("User.u3.worker", 1); }
    libfn(0);   // cannot reuse the cache entry -> create a new one
  }

  proc main {
    u1();
    u2();
    u3();
    {
      use super.More1;
      m1con();
      m1gen(0);
    }
    {
      use super.More2;
      m2con();
      m2gen(0);
    }
    {
      use super.Combo1;
      combo1a();
    }
    {
      use super.Combo2;
      combo2b();
    }
    compilerError("done",0);
  }
}

module More1 {
  use super.Lib;
  proc worker() { compilerWarning("More1.worker", 1); }

  proc m1con() {
    libfn(0);   // cannot reuse the cache entry -> create a new one
  }
  proc m1gen(param p) {
    libfn(0);   // can reuse the cache entry created for m1con
  }
}

module More2 {
  use super.Lib;

  proc m2con() {
    // libfn(0);   // error: no visible worker()
  }
  proc m2gen(param p) {
    libfn(0);   // can reuse the cache entry created for u1
  }
}

module Combo1 {
  use super.Lib;
  proc worker() { compilerWarning("Combo1.worker", 1); }

  proc combo1a() {
    use super.Combo2;
    combo2a(0);
  }
  proc combo1b(param p) {
    libfn(0);   // can reuse the cache entry created for combo1a->combo2a
  }
}

module Combo2 {
  use super.Lib;

  proc combo2a(param p) {
    libfn(0); // create a fresh cache entry
  }
  proc combo2b() {
    use super.Combo1;
    combo1b(0);
  }
}

} // module Work
