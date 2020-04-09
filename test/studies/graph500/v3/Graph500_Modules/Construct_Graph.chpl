module Construct_Graph
{
// Code to construct graph from the Edges (the array of Chapels records 
// containing the vertex pairs )


proc constructGraph(Edges:[?ArrD] , G) 
{
   use Graph500_defs;
   use Sort;

   var Histogram$ : [G.my_vertices] atomic int; // initialized to 0 by default, right?

   // Generate a histogram from the Edges to guide the distribution
   // of the graph

   forall e in Edges do {
      var u = e.start;
      var v = e.end;
      Histogram$[u].add(1);
      Histogram$[v].add(1);
   }

   // Resize Neighbor lists for each vertex based on Histogram
/* use zippering instead:
   forall v in G.my_vertices {
       G.Vertices[v].nd = {1..Histogram$[v].read()};
   }
*/
   // zippered version of the above
   forall (vertex, histogram) in zip(G.Vertices, Histogram$) do
     vertex.nd = {1..histogram.read()};

// Note that graph for Graph500 benchmark is undirected
// Self edges are removed, duplicates are noted

   forall e in Edges do {
     var u: vertex_id = e.start;
     var v: vertex_id = e.end;

     if ( v != u ) then {
/* do not check for duplicates - do it later instead
       if G.Vertices (u).is_a_neighbor(v) then {
         G.Vertices (u).add_duplicate();
       }
       else
*/
       {
         G.Vertices (u).add_Neighbor (v);
         G.Vertices (v).add_Neighbor (u);
       }

     }

     else {
       // just bump the count
       G.Vertices (u).add_self_edge();
     }
   }

   forall v in G.Vertices do {
     const oldNC = v.neighbor_count.read();
     // TODO: remove duplicates via sorting
     // sort(v.nd); ...
     const newNC = oldNC; // TODO: recompute after duplicate removal
     if newNC != v.nd.size {
       // resize
       v.nd  = {1..newNC};
     }
   }


      if DEBUG_GRAPH_GENERATOR then {

//      Compute total number of edges in the graph

        var total_edges = (+ reduce [v in G.Vertices] v.neighbor_count )/2;
        writeln ( "# of edges in final graph ", total_edges);
        var self_edges = (+ reduce [v in G.Vertices] v.self_edges );
        writeln ( "# of self edges           ", self_edges );
/* TODO: compute these (not done currently)
        var duplicates = (+ reduce [v in G.Vertices] v.duplicates );

        writeln ( "# of duplicates           ", duplicates );
*/

//      Generate histogram of node distributions by number of outgoing edges        
        var max_edges = max reduce [v in G.Vertices] v.neighbor_count.read();
        writeln (" Maximum size of Neighbor list over all vertices: ",max_edges);

      if DEBUG_EDGE_HISTOGRAM {
        var edge_count : [0..max_edges] atomic int = 0;

        forall v in G.Vertices do
           edge_count (v.neighbor_count).add(1);

        writeln ("Histogram of nodes by number of outgoing edges");
        writeln (" # of edges   # of nodes ");
        for count in 0..max_edges do
         {
          const ec = edge_count(count).read();
          if ec != 0 then writeln (count, "  ", ec );
         }
       } // if DEBUG_EDGE_HISTOGRAM

      }

}

}
