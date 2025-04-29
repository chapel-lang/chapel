use BlockDist;

var ArrMatch = blockDist.createArray(1..10, int);
var ArrOdd = blockDist.createArray(1..11, int);

forall i in ArrMatch.domain {
  ArrMatch[i] = 
    ArrOdd[i] +
    ArrOdd[i+1];
}

writeln(ArrMatch);
