//
// An interesting thing to note about opaque domains/arrays is that in
// order to support graphs with data associated with edges and
// weights, a domain is required for the Vertices and the Edges, since
// each will require an array to be declared over it.  Thus:
//

var Vertices: domain(opaque);
var Edges: domain(opaque);

config const numVertices = 10;

var Label: [Vertices] string;
var numOutEdges: [Vertices] int;
var VertexWeight: [Vertices] real;

var outEdges: [Vertices] [1..numVertices] index(Edges);
var from, to: [Edges] index(Vertices);
var EdgeWeight: [Edges] real;

createRandomGraph();

writeln("Created the random graph");

exit(0);

for v in Vertices {
  writeln(Label(v), " has a weight of ", VertexWeight(v), " and links to: ");
  if (numOutEdges(v) == 0) then
    writeln("<nothing>");
  else
    for i in 1..numOutEdges(v) do
      writeln(" ", Label(to(outEdges(v)(i))));
}


for e in Edges do
  writeln("an edge with weight ", EdgeWeight(e), " links from ", Label(from(e)),
          " to ", Label(to(e)));



proc createRandomGraph() {
  // temporary index for storing vertices during graph generation
  //  var vertices: [1..numVertices] index(Vertices);

  // set up random number stream
  use Random;
  var myRandNums = new borrowed RandomStream(real, seed=314159265);

  // allocate vertices
  for i in 1..numVertices {
    //    vertices(i) = Vertices.create();
    const newVertex = Vertices.create();
    Label(newVertex) = "v" + i:string;
    VertexWeight(newVertex) = myRandNums.getNext();
  }

  // iterate over all pairs of vertices
  for vi in Vertices {
    for vj in Vertices {

      // roll a random number; if it's between 0.5 and 1.0, add an edge (vi, vj)
      const randVal = myRandNums.getNext();
      if (randVal > 0.5) {
        // allocate a new edge index
        const newEdge = Edges.create();
        EdgeWeight(newEdge) = myRandNums.getNext();

        // increment the number of out edges for the source vertex vi and grab
        // the edge
        numOutEdges(vi) += 1;
        outEdges(vi)(numOutEdges(vi)) = newEdge;

        // store the source and sink vertices for the edge
        from(newEdge) = vi;
        to(newEdge) = vj;
      }
    }
  }
}

