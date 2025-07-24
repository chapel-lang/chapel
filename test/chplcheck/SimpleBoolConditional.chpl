module SimpleBoolConditional {
  proc foo(x) {
    return if x /**/ == 1 then true else false;
  }
  proc fooInv(x) {
    return if x == 1 then false else true;
  }
  proc bar(x) {
    if x == /**/1 then
      return true;
    else
      return false;
  }
  proc barInv(x) {
    if x == 1 then
      return false;
    else
      return true;
  }
  proc egg(x) {
    if x == 1 {
      return true;
    } else {
      return false;
    }
  }
  proc eggInv(x) {
    if x /**/== 1 {
      return false;
    } else {
      return true;
    }
  }
  proc spam(x) {
    var a = if x == 1 then true else false;
    var b = if x == 2 then false else true;

    const c = if x == 3 then true else false,
          d = if x == 4 then false else true;
    return a && b && c && d;
  }
}
