class Subgraph {
  type wtype;
  var x : wtype;
}

var x : Subgraph(wtype = integer) = Subgraph(wtype = integer);

x.x = 1;

writeln(x.x);
