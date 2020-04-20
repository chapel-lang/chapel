use NodesEdges, Edges, Nodes, BC_dijkstra, Time, IO;

config const input_file: string = "RtsGraph_OneOverP.txt";
config const gen_file: string = "RtsGraph_gen.txt";
config const load_file: string = "RtsGraph_load.txt";
config const isDirected: int;

proc main() {

  var nEdges: int;  // represents first line in file -- the number of entries
  var nNodes: int;

  var fin = open(input_file, iomode.r).reader();
  var fout = open("RtsGraph_out.txt", iomode.cw).writer();
  var fin2 = open(gen_file, iomode.r).reader();
  var fin3 = open(load_file, iomode.r).reader();

  var init_tm: real;
  const init_t0 = getCurrentTime();

  fin.readln(nEdges);

  var D1 = {0..(nEdges-1)};
  var U: [D1] int;
  var V: [D1] int;
  var W: [D1] real;

  for i in D1 {
    fin.readln(U[i]);
    fin.readln(V[i]);
    fin.readln(W[i]);
  }

  fin.close();

  // Initialize edge list and set number of nodes
  var Edges = InitializeEdges(nEdges, nNodes, U, V, W);

  // Initialize node data structure
  var Nodes = InitializeNodes(nEdges, nNodes, Edges, isDirected);

  var nSources: int;
  var ignoreWt: real;

  //Initialize Generators/Sources:
  fin2.readln(nSources);

  writeln("Reading Sources: ", nSources);
  var D2 = {0..(nSources-1)};
  var Sources: [D2] int;

  for i in D2 {
    fin2.readln(Sources[i]);
    fin2.readln(ignoreWt);
    Sources[i] -= 1;                  // Adjust for 0-based indexing
    Nodes[Sources[i]].nodeType = 0;   // Mark the node as a source
    writeln(Sources[i], " - ", ignoreWt);
  }
  fin2.close();

  var nSinks: int;
  fin3.readln(nSinks);

  writeln("Reading Sinks: ", nSinks);
  var D3 = {0..(nSinks-1)};
  var Sinks: [D3] int;

  for i in D3 {
    fin3.readln(Sinks[i]);
    fin3.readln(ignoreWt);
    Sinks[i] -= 1;                    // Adjust for 0-based indexing
    Nodes[Sinks[i]].nodeType = 1;     // Mark the node as a sink
    writeln(Sinks[i], " - ", ignoreWt);
  }
  fin3.close();

  init_tm = getCurrentTime() - init_t0;
//  writeln("Initialization time: ", init_tm);

  displayGraph(Nodes, nNodes);
  displaySourceSink(Sources, nSources, Sinks, nSinks);

  // Calculate vertex betweenness of node i, and update edge betweenness
  //   using an extended dijkstra algorithm
  writeln("Dijkstra Algorithm: Source to All");

  var dijkstra_tm: real;
  const dijkstra_t0 = getCurrentTime();

  var D4 = {0..(nNodes-1)};
  forall i in D4 {
    dijkstra(i, nEdges, nNodes, Edges, Nodes);   // All-pairs
  }

  dijkstra_tm = getCurrentTime() - dijkstra_t0;
//  writeln("Compute time: ", dijkstra_tm);

  for i in D1 {
    if (Edges[i]!.dupl != 0) {

      // Recover original distance of edge
      var sum: real = 1.0 / Edges[i]!.distance;
      for j in (i+1)..(nEdges-1) {
        if (Edges[j]!.dupl == 1) then break;
        sum -= 1.0 / Edges[j]!.distance;
      }

      Edges[i]!.distance = 1.0 / sum;

      // Compute total distance of component edges (= total distance)
      sum = Edges[i]!.distance;
      for j in (i+1)..(nEdges-1) {
        if (Edges[j]!.dupl == 1) then break;
        sum += Edges[j]!.distance;
      }

      // Divide hyperlink score to component edges proportional
      // to (edge distance / total distance)
      var vbHL: real = Edges[i]!.vb$.readXX() / sum;
      Edges[i]!.vb$.writeXF(Edges[i]!.distance * vbHL);

      for j in (i+1)..(nEdges-1) {
        if (Edges[j]!.dupl == 1) then break;
        Edges[j]!.vb$.writeXF(Edges[j]!.distance * vbHL);
      }
    }
  }

  writeln("#     From     To     Betweenness");

  for i in D1 do writeln(Edges[i]!.id, "     ", Edges[i]!.n1+1, "     ",
                         Edges[i]!.n2+1, "     ", Edges[i]!.vb$.readXX());

  for n in Nodes do delete n;
  for e in Edges do delete e;
}
