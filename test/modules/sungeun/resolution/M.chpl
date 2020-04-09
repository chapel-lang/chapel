module M {
  use M0;
  writeln("hi from M!");
  record R {
    var x: t;
  }

  inline proc _cast(type t, x: int) where t==R {
    var ret: R;
    ret.x = x;
    return ret;
  }
}
