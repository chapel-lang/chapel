
var first: bool = true;
proc main {
  var aliasDom: domain(1) = {1..5};
  var notAliasDom: domain(1) = {1..20};
  var A: [notAliasDom] int = [i in notAliasDom] i;

  {
  ref B = A(1..5).reindex(aliasDom);
  writeArr(B);
  B = 1;
  writeArr(A);
  reset(A);
  }
  {
  ref B = A(6..10).reindex(aliasDom);
  writeArr(B);
  B = 2;
  writeArr(A);
  reset(A);
  }
  {
  ref B = A(1..20 by 4).reindex(aliasDom);
  writeArr(B);
  B = 3;
  writeArr(A);
  reset(A);
  }
  {
  ref B = A(7.. by 2 #5).reindex(aliasDom);
  writeArr(B);
  B = 4;
  writeArr(A);
  reset(A);
  }

  {
  ref C = A(1..5).reindex(aliasDom.translate(1));
  writeArr(C);
  C = 1;
  writeArr(A);
  reset(A);
  }
  {
  ref C = A(6..10).reindex(aliasDom.translate(1));
  writeArr(C);
  C = 2;
  writeArr(A);
  reset(A);
  }
  {
  ref C = A(1..20 by 4).reindex(aliasDom.translate(1));
  writeArr(C);
  C = 3;
  writeArr(A);
  reset(A);
  }
  {
  ref C = A(7.. by 2 #5).reindex(aliasDom.translate(1));
  writeArr(C);
  C = 4;
  writeArr(A);
  reset(A);
  }
}

proc reset(A: [] int) {
  [i in A.domain] A(i) = i;
}

proc writeArr(A: [] int) {
  if first {
    writeln("------");
    first = false;
  }
  for i in A.domain do
    write((i, A(i)), " ");
  writeln();
  writeln("------");
}
