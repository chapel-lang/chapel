use Collectives;

var b = new barrier(4);

if b.check() then
  writeln("check returned true?");
