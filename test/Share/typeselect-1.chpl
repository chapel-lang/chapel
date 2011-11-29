union Weigth {
  var i : int;
  var s : string;
}

var edges[1..3] : Weight;
edges(1) = 1:Weigth.i;
edges(2) = 2:Weigth.i;
edges(3) = "three":Weigth.s;

var sought_string;
typeselect(w = edges(3).weight) {
   when s    sought_string = w;
}
