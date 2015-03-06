// The module wrapper is required because module names and type names can conflict.
// This is a known bug: test/modules/hilde/Writer.future
module Opaque {
/*
 * Opaque Primer
 *
 * This primer is designed to give a brief introduction to Chapel's
 * opaque domains and arrays.  Opaque domains and arrays are those in
 * which index values are opaque/anonymous, and were designed to
 * support unstructured arrays like graphs.
 */

//
// Opaque domains are declared by specifying the keyword "opaque" in
// place of the normal domain index type and rank specifiers.  In
// this example, we'll use an opaque domain to represent a group of
// people.
//

var People: domain(opaque);

//
// Since opaque domains don't support logical index values, new
// indices are created by requesting them from the dommain directly.
// So, to add our first three people to the People domain, we could
// do the following declarations/assignments:
//

var person1 = People.create();  // person inferred to be of type index(People);
var person2: index(People) = People.create();
var person3: index(opaque) = People.create();

//
// Like other domain types, arrays can be declared using opaque
// domains and accessed using indices from the domain:
//

var Name: [People] string;

Name(person1) = "Barry";
Name(person2) = "Bill";
Name(person3) = "Bob";

//
// Since opaque indices don't have logical values, printing an opaque
// domain is not particularly useful.  Opaque arrays can be printed out
// like any other array.  Like associative domains, there are no
// guarantees about the order in which elements will be printed.
//

writeln("People is: ", People);
writeln("Name is: ", Name);
writeln("Name(person2) is: ", Name(person2));
writeln();

//
// Similarly, the order in which opaque domain/array elements are
// accessed in a for loop is arbitrary, though it will not change
// between modifications to the domain:
//

for person in People do
  writeln("Name(person) = ", Name(person));
writeln();

//
// To help with this ordering issue, opaque domains support an
// iterator named "sorted()" which pre-sorts the values and
// traverses them in order.  Needless to say, this costs extra
// time and space, so is meant only as a convenience.
//

writeln("Name (sorted) is: ", Name.sorted());
writeln();

for name in Name.sorted() do
  writeln("Name (in sorted order) is: ", name);
writeln();

//
// Now let's say we want to use this opaque domain to describe a graph.
// In order to do this, we need to store opaque indices for each index
// in the domain.  For example, let's assume that Barry is the father of
// Bill and Bob.  We could represent this by declaring an array of
// indices from the People domain:
// 

var Father: [People] index(People);

//
// This declaration says "for every index in the People domain, store
// an index from the People domain.  We can then assign this array
// like any other:
//

Father(person2) = person1;
Father(person3) = person1;

//
// The default value of an opaque index is nil.  Thus, having set up
// the Father relationship described above, we can print it out as
// follows:
//

for person in People {
  write("Father(", Name(person), ") is ");
  if (Father(person) == nil) {
    writeln("unknown");
  } else {
    writeln(Name(Father(person)));
  }
}
writeln();

//
// Next, let's create a "Child" array so that each person can refer to
// all of their children:
//

config const maxChildren = 16;

var NumChildren: [People] int;
var Child: [People] [1..maxChildren] index(People);

proc addChild(parent: index(People), child: index(People)) {
  const childnum = NumChildren(parent) + 1;
  NumChildren(parent) = childnum;
  Child(parent)(childnum) = child;
}

addChild(person1, person2);
addChild(person1, person3);

//
// And now we can print out the child information:
//

for person in People {
  write(Name(person));
  if (NumChildren(person) == 0) then
    writeln(" has no children");
  else {
    write("'s children are:");
    for i in 1..NumChildren(person) do
      write(" ", Name(Child(person)(i)));
    writeln();
  }
}
writeln();

//
// Note that the graphs above have data associated with their nodes
// but not their edges.  An interesting note about opaque
// domains/arrays is that in order to support graphs with data
// on edges and nodes, a domain is required for the Vertices and the
// Edges, since each will need to support an array of data.
//
// As an example, the following code generates and prints a random
// graph:
//

//
// Declare two domains, one for the vertices, one for the edges
//
var Vertices: domain(opaque);
var Edges: domain(opaque);

config const numVertices = 5;

//
// For each vertex, we'll store a string label, a count of its number
// of out edges, and its weight.
//
var Label: [Vertices] string;
var numOutEdges: [Vertices] int;
var VertexWeight: [Vertices] real;

//
// We'll also store a list of the edges that lead out of it
//
var outEdges: [Vertices] [1..numVertices] index(Edges);

//
// For the edges, we'll store a weight
//
var EdgeWeight: [Edges] real;

//
// And an indication of the vertices that they lead from and to
//
var from, to: [Edges] index(Vertices);

//
// Generate a random graph using the routine below
//
createRandomGraph();

//
// Print out the graph from a vertex-centric point of view...
//
writeln("Vertex-oriented view of random graph");
writeln("------------------------------------");
for v in Vertices {
  //
  // print out the vertex and its weight
  //
  write(Label(v), " has a weight of ", VertexWeight(v), " and links to: ");
  //
  // print the enumeration of its neighbors, if any
  //
  if (numOutEdges(v) == 0) then
    writeln("<nothing>");
  else {
    for i in 1..numOutEdges(v) do
      write(" ", Label(to(outEdges(v)(i))));
    writeln();
  }
}
writeln();

//
// Print out the graph from an edge-centric point of view...
//
writeln("Edge-oriented view of random graph");
writeln("----------------------------------");
for e in Edges do
  //
  // For each edge, print out its weight, source, and sink
  //
  writeln("an edge with weight ", EdgeWeight(e), " links from ", Label(from(e)),
          " to ", Label(to(e)));
writeln();



//
// create a random graph using the global variables defined above
//
proc createRandomGraph() {
  //
  // create a random number stream
  //
  // Note: example usage of the standard module Random can be found in
  // the primer randomNumbers.chpl, located in the current directory.
  //
  use Random;
  var myRandNums = new RandomStream(seed=314159265);

  //
  // allocate all the vertices and assign them labels and random weights
  //
  for i in 1..numVertices {
    const newVertex = Vertices.create();
    Label(newVertex) = "v" + i;
    VertexWeight(newVertex) = myRandNums.getNext();
  }

  //
  // iterate over all pairs of vertices
  //
  for vi in Vertices {
    for vj in Vertices {
      //
      // roll a random number; if it's between 0.5 and 1.0, add edge (vi, vj)
      //
      if (myRandNums.getNext() > 0.5) {
        //
        // allocate a new edge and give it a random weight
        //
        const newEdge = Edges.create();
        EdgeWeight(newEdge) = myRandNums.getNext();

        //
        // increment the number of out edges for the source vertex vi and add
        // the edge to its list of out edges
        //
        numOutEdges(vi) += 1;
        outEdges(vi)(numOutEdges(vi)) = newEdge;

        //
        // store the source and sink vertices for the edge
        //
        from(newEdge) = vi;
        to(newEdge) = vj;
      }
    }
  }

  delete myRandNums;
}

}
