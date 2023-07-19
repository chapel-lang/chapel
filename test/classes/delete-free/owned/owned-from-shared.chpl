
class CC { var x: int; }

proc main {
  var oo = new shared CC(5)?;
  var ss = owned.adopt(oo);
  writeln(oo == nil);
}
