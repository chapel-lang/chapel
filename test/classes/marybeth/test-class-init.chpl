class Params {
  var numProbSizes: int;
  var N: [1..numProbSizes] int;

  def initialize() {
    numProbSizes = 3;
    for n in N do n = 10;
  }
}

var ListOfParams = Params();

writeln(ListOfParams);
