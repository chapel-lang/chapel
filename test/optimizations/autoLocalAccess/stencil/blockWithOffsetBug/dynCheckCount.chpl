use StencilDist;

var ArrMatch = stencilDist.createArray(1..10, int);
var ArrOdd = stencilDist.createArray(1..11, int);

forall i in ArrMatch.domain {
  ArrMatch[i] = 
    ArrOdd[i+1] +
    ArrOdd[i+1] +
    ArrOdd[i+1] +
    ArrOdd[i];
}

writeln(ArrMatch);
