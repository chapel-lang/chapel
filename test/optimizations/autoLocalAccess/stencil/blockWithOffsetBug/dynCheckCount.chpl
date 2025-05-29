// the purpose of this test is to make sure that we call the main dynamic ALA
// check (not the offset one) only once
use StencilDist;

var ArrMatch = stencilDist.createArray(1..10, int);
var ArrOdd = stencilDist.createArray(1..11, int);

ArrOdd = 1;

forall i in ArrMatch.domain {
  ArrMatch[i] =
    ArrOdd[i+1] +
    ArrOdd[i+1] +
    ArrOdd[i+1] +
    ArrOdd[i];
}

writeln(ArrMatch);
