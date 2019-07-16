module M {
  var f = try! open("throwingMethodWithDefaultArg.chpl", iomode.r);
  var ch = try! f.reader();
}
