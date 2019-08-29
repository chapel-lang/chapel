
config param case : int = 0;

proc makeValue() {
  return "hello";
}

proc makeType() type {
  return int;
}

class BadTypeDefault {
  type T = makeValue();
}

class BadParamDefault {
  param x = makeValue();
}

class BadValueDefault {
  var x = makeType();
}

proc main() {
  if case == 0 {
    type T = borrowed BadTypeDefault?;
    var x:T;
  } else if case == 1 {
    type T = borrowed BadParamDefault?;
    var x:T;
  } else if case == 2 {
    type T = borrowed BadValueDefault?;
    var x:T;
  }
}
