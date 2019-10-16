var graph : domain(opaque);
var node1 = graph.create();
var node2 = graph.create();
var node3 = graph.create();
if graph.contains(node1) then
    graph.remove(node1);
writeln(graph);
