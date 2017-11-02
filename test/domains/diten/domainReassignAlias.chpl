proc main {
  var D1 = {1..10};
  var D2 = {1..10};
  var A1: [D1] int = D1;
  ref A2 = A1.reindex(D2);
  writeln("A1=", A1);
  writeln("A2=", A2);
  D1 = {1..11};
  writeln("A1=", A1);
  writeln("A2=", A2);
}
