
class CC { var x: int; }

proc main {
  var oo = new owned CC(5);
  var ss = shared.create(oo);
  writeln(oo == nil);
}
