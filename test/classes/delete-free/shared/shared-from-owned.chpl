
class CC { var x: int; }

proc main {
  var oo = new owned CC(5);
  var ss = shared.adopt(oo);
  writeln(oo == nil);
}
