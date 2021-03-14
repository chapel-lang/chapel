module X {
  module M {
    record R { // M.R
      type t;
      var x : int;
    }
  }

  use M;

  record R { // X.R
    var r : R(int); // 'R' should refer to X.R, not M.R
  }

  var r : R;
  writeln(r);
}

