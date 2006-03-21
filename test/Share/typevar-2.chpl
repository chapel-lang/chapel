class Subgraph {
  type wtype;
  var x : wtype;
  constructor Subgraph(y : int) : x(y) {
  }
}

var x = Subgraph(5);

x.x = 1;

writeln(x.x);
