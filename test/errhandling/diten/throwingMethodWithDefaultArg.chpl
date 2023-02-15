module M {
  use IO;

  var f = try! open("throwingMethodWithDefaultArg.chpl", ioMode.r);
  var ch = try! f.reader();
}
