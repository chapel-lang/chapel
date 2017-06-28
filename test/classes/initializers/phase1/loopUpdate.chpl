class Params {
  var numProbSizes: int;
  var N: [1..numProbSizes] int;

  proc init() {
    numProbSizes = 3;
    for n in N do n = 10;
    super.init();
  }
}

var ListOfParams = new Params();

writeln(ListOfParams);

delete ListOfParams;
