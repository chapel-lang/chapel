record Bar {
  var x: int;

  proc type defaultVal { return new Bar(11); }
}

proc throwingFunc(x = Bar.defaultVal): Bar throws {
  throw new Error("whatev");
  return x;
}

proc callThrowingFunc() {
  var y = try throwingFunc();
}

proc main() {
  callThrowingFunc();
}
