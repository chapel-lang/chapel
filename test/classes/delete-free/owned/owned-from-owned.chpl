
class CC { var x: int; }

proc main {
  var oo = new owned CC(5);
  var ss = oo;
  writeln(oo == nil);
}
