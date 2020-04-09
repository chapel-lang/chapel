class Params {
  var numProbSizes: int;
  var N: [1..numProbSizes] int;

  proc postinit() {
    numProbSizes = 3;
    for n in N do n = 10;
  }
}

var ListOfParams = new unmanaged Params();

writeln(ListOfParams);

delete ListOfParams;
