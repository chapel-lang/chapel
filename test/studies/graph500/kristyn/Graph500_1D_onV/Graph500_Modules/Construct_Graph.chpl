module Construct_Graph
{
// Code to construct graph from the Edges (the array of Chapels records 
// containing the vertex pairs )


proc constructGraph(Edges:[?ArrD] , G) 
{
   use Graph500_defs;

   var Histogram$ : [G.my_vertices] sync int = 0;

   // Generate a histogram from the Edges to guide the distribution
   // of the graph

   forall e in Edges do {
      var u = e.start;
      var v = e.end;
      Histogram$[u] += 1;
      Histogram$[v] += 1;
   }

   // Resize Neighbor lists for each vertex based on Histogram

   forall v in G.my_vertices {
       G.Vertices[v].nd = {1..Histogram$[v].readFF()};
   }


// Note that graph for Graph500 benchmark is undirected
// Self edges are removed, duplicates are noted

   forall e in Edges do {
     var u: vertex_id = e.start;
     var v: vertex_id = e.end;

     if ( v != u ) then {
       if G.Vertices (u).is_a_neighbor(v) then {
         G.Vertices (u).add_duplicate();
       }
       else {
         G.Vertices (u).add_Neighbor (v);
         G.Vertices (v).add_Neighbor (u);
       }

     }

     else {
       G.Vertices (u).add_self_edge();
     }
   }

   forall v in G.Vertices do {
     v.nd  = {1..v.neighbor_count};
   }


      if DEBUG_GRAPH_GENERATOR then {

//      Compute total number of edges in the graph

        var total_edges = (+ reduce [v in G.Vertices] v.neighbor_count )/2;
        writeln ( "# of edges in final graph ", total_edges);
        var self_edges = (+ reduce [v in G.Vertices] v.self_edges );
        writeln ( "# of self edges           ", self_edges );
        var duplicates = (+ reduce [v in G.Vertices] v.duplicates );

        writeln ( "# of duplicates           ", duplicates );

//      Generate histogram of node distributions by number of outgoing edges        
        var max_edges = max reduce [v in G.Vertices] v.neighbor_count;
        writeln (" Maximum size of Neighbor list over all vertices: ",max_edges);

        var edge_count : [0..max_edges] int = 0;

        for v in G.Vertices do
           edge_count (v.neighbor_count) += 1;

        writeln ("Histogram of nodes by number of outgoing edges");
        writeln (" # of edges   # of nodes ");
        for count in 0..max_edges do
          writeln (count, "  ", edge_count (count) );

      }

}

}
