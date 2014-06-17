
var first: bool = true;
proc main {
  var aliasDom: domain(1) = {1..5};
  var notAliasDom: domain(1) = {1..20};
  var A: [notAliasDom] int = [i in notAliasDom] i;

  {
  var B: [aliasDom] => A(1..5);
  writeArr(B);
  B = 1;
  writeArr(A);
  reset(A);
  }
  {
  var B: [aliasDom] => A(6..10);
  writeArr(B);
  B = 2;
  writeArr(A);
  reset(A);
  }
  {
  var B: [aliasDom] => A(1..20 by 4);
  writeArr(B);
  B = 3;
  writeArr(A);
  reset(A);
  }
  {
  var B: [aliasDom] => A(7.. by 2 #5);
  writeArr(B);
  B = 4;
  writeArr(A);
  reset(A);
  }

  {
  var C: [aliasDom.translate(1)] => A(1..5);
  writeArr(C);
  C = 1;
  writeArr(A);
  reset(A);
  }
  {
  var C: [aliasDom.translate(1)] => A(6..10);
  writeArr(C);
  C = 2;
  writeArr(A);
  reset(A);
  }
  {
  var C: [aliasDom.translate(1)] => A(1..20 by 4);
  writeArr(C);
  C = 3;
  writeArr(A);
  reset(A);
  }
  {
  var C: [aliasDom.translate(1)] => A(7.. by 2 #5);
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
