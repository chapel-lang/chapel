// these are needed to compile the standard library,
// since this module is replacing the standard AutoMath.

proc max(a: int, b: int) {
  if a > b {
    return a;
  } else {
    return b;
  }
}
proc max(param a: int, param b: int) param {
  if a > b {
    return a;
  } else {
    return b;
  }
}

proc min(a: int, b: int) {
  if a < b {
    return a;
  } else {
    return b;
  }
}
proc min(param a: int, param b: int) param {
  if a < b {
    return a;
  } else {
    return b;
  }
}

proc abs(a: int) {
  if a < 0 {
    return -a;
  } else {
    return a;
  }
}

proc testmath() {
  writeln("In my math!");
}

