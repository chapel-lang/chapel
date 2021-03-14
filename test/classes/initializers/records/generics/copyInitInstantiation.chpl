module SomeRecord {
  record SomeR {
    type t;
    var x : t;

    proc init(type t) {
      this.t = t;
    }
  }
}

module MockStandard {
  use SomeRecord;

  proc helper(type t) {
    var r = new SomeR(t);

    // Compiler-generated copy initializer needs to know about R.
    //
    // Program would fail to compile if SomeR's copy initializer had an
    // incorrect point-of-instantiation.
    var x = r;

    writeln("x = ", x);
  }
}

module MockMain {
  use MockStandard;
  record R {
    var x : int;
    proc init() {
      writeln("R.init");
    }
  }
  proc main() {
    helper(R);
  }
}

