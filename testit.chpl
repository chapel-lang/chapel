use BlockDist, Time;

const D = {1..10} dmapped Block(rank=1);
writeln("D is: ", D);
D.displayRepresentation();
D.dist.displayRepresentation();
on Locales[numLocales-1] {
  writeln("From locale 1");
  writeln("D is: ", D);
  D.displayRepresentation();
  writeln("writing the dist's representation");
  D.dist.displayRepresentation();
  writeln("-----");
}
writeln("-----");
var A: [D] real;

forall a in A do
  a = here.id;

writeln(A);
