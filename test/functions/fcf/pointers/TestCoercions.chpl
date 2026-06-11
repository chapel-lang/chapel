
class C {
  var x : int;
}

proc procA(arg: borrowed C) {
  writeln("procA: ", arg.x);
}

proc procB(arg: borrowed C) {
  writeln("procB: ", arg.x);
}

proc test(cond: bool) {
  var fn = if cond then procA else procB;
  var c = new owned C(42);
  procA(c);
  procB(c);
  fn(c);
}

proc main() {
  test(true);
}
