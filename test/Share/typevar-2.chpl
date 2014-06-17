class Subgraph {
  type wtype;
  var x : wtype;
  proc Subgraph(y : int) : x(y) {
  }
}

var x = new Subgraph(5);

x.x = 1;

writeln(x.x);
