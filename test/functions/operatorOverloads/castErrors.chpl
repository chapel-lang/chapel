record R {
}

record S {
}

operator :(x: R, type t: S, y: R) {
  return new S();
}

operator :(x: R) {
  return new S();
}

operator :(x: R, y: S) {
  return new S();
}

record T {
  operator :(x: R, type t: S, y: R) {
    return new S();
  }

  operator :(x: R) {
    return new S();
  }

  operator :(x: R, y: S) {
    return new S();
  }
}
  
var r: R;
var s: S;
var t: T;
