class Subgraph {
  type wtype;
  var x : wtype;
}

var x : Subgraph(wtype = int) = Subgraph(wtype = int);

x.x = 1;

writeln(x.x);
