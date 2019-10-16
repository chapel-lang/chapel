
proc makeParam() param {
  return 5;
}

proc makeValue() {
  return "hello";
}

class BadParam {
  param x : makeParam();
}

class BadValue {
  var x : makeValue();
}

config param case : int = 0;

proc main() {
  if case == 0 {
    type T = BadParam(5);
  } else if case == 1 {
    type T = BadValue;
  }
}
