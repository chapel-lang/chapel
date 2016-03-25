use Graph;

var g:Graph;
g = new Graph(4);

g.addEdge(0, 1);
g.addEdge(0, 2);
g.addEdge(1, 2);
g.addEdge(2, 0);
g.addEdge(2, 3);
g.addEdge(3, 3);

writeln("Following is Breadth First Traversal (starting from vertex 2)");

g.bfs(2);
