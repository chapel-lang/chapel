module M {
  use M0;
  writeln("hi from M!");
  record R {
    var x: t;
  }

  inline operator :(x: int, type t:R) {
    var ret: R;
    ret.x = x;
    return ret;
  }
}
