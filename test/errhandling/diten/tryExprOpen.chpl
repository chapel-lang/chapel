module M {
  var f = try! open("trybang.chpl", iomode.r);
  var ch = try! f.reader();
}
