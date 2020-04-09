class Params {
  var numProbSizes: int;
  var N: [1..numProbSizes] int;

  proc init() {
    numProbSizes = 3;
    for n in N do n = 10;
    forall n in N do n = 10;
    forall (n1, n2) in zip(N, N) do n1 = 10;
  }
}

var ListOfParams = new Params();

writeln(ListOfParams);

delete ListOfParams;
