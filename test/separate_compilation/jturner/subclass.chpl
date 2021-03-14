class Parent {
  var i : int;

  proc dothat(j:int) {
    return j + 3;
  }
}

class Child : Parent {
  proc dothat(j:int) {
    return j + 10;
  }
}

export proc dothis(p : Parent) : int {
  return p.dothat(p.i);
}
