var first: bool = true;
def main {
  var aliasDom: domain(1, alias=true) = [1..5];
  var notAliasDom: domain(1) = [1..20];
  var A: [i in notAliasDom] int = i;
  var B: [aliasDom] int;
  var C: [aliasDom + 1] int;

  B => A(1..5);
  writeArr(B);
  B = 1;
  writeArr(A);
  reset(A);
  B => A(6..10);
  writeArr(B);
  B = 2;
  writeArr(A);
  reset(A);
  B => A(1..20 by 4);
  writeArr(B);
  B = 3;
  writeArr(A);
  reset(A);
  B => A(7.. by 2 #5);
  writeArr(B);
  B = 4;
  writeArr(A);
  reset(A);

  C => A(1..5);
  writeArr(C);
  C = 1;
  writeArr(A);
  reset(A);
  C => A(6..10);
  writeArr(C);
  C = 2;
  writeArr(A);
  reset(A);
  C => A(1..20 by 4);
  writeArr(C);
  C = 3;
  writeArr(A);
  reset(A);
  C => A(7.. by 2 #5);
  writeArr(C);
  C = 4;
  writeArr(A);
  reset(A);
}

def reset(A: [] int) {
  [i in A.domain] A(i) = i;
}

def writeArr(A: [] int) {
  if first {
    writeln("------");
    first = false;
  }
  for i in A.domain do
    write((i, A(i)), " ");
  writeln();
  writeln("------");
}
