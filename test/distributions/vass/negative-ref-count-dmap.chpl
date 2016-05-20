
use BlockDist;

const d1 = {1..4};

proc main() {
  var bd1 = new Block(d1);
  var dm1 = new dmap(bd1);
  writeln(77777777);
  writeln(for tl in bd1.targetLocales do tl.id);
  writeln(88888888);
}
