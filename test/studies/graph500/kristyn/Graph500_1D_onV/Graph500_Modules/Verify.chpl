module Verify
{

proc verify_bfs_tree (root : int, ParentTree, Edges): int
{

   use Graph500_defs;

   var error: int=0;
   var n_edges_traversed: int =0;

   var Level: [vertex_domain] vertex_id=-1;
   Level [root] = 0;

   // Check min/max Node ID's

   var min_vertex_ID_start = min reduce Edges.start;
   var min_vertex_ID_end = min reduce Edges.end;
   var max_vertex_ID_start = max reduce Edges.start;
   var max_vertex_ID_end = max reduce Edges.end;

   var min_vertex_ID = min (min_vertex_ID_start, min_vertex_ID_end);
   var max_vertex_ID = max (max_vertex_ID_start, max_vertex_ID_end);

//   writeln ("Minimum vertex ID ", min_vertex_ID);
//   writeln ("Maximum vertex ID ", max_vertex_ID);

   error = compute_levels ( ParentTree, Level);

//   writeln(" Level array", Level); 

   if (error < 0) then return error;

   var Seen_Edge: [vertex_domain] int=0;

// Note: having all threads monitor error to see when this gets set could 
// create a significant hotspot if run in parallel

   for e in Edges do {

     if ( error == 0 ) {

       var u = e.start;
       var v = e.end;

      // If both the starting vertex or end vertex are in the tree 
      // this counts as a travered edge. Self edges and duplicates are also
      // counted

       if (ParentTree[u] >  0 && ParentTree[v] > 0){
         n_edges_traversed += 1;
       }
       else 
       {
 
         if (ParentTree[u] >  0 && ParentTree[v] < 0)
         {  
           error = -12;
         }
         if (ParentTree[u] <  0 && ParentTree[v] > 0)
         {  
           error = -13;
         }
       }

       // Mark seen tree edges
       if ( u != v) {
         if (ParentTree[u] == v) then Seen_Edge[u] = 1;
         if (ParentTree[v] == u) then Seen_Edge[v] = 1;
       }

       // Check that levels differ by no more than one.
       var lvldiff: int = Level[u] - Level[v];
       if (lvldiff > 1 || lvldiff < -1) {
         error = -14;
       }
     }

   }

   // Check that every BFS edge was seen and that there is only one root

   if (error == 0) {
     for k in vertex_domain {
       if (error == 0) {
         if (k != root) {
           if (ParentTree[k] >= 0 && Seen_Edge[k] == 0)
           {
             error = -15;
           }
           else 
           {
             if ( ParentTree[k] == k) then error = -16;
           }
         }
       }
     }
   }
      

//   writeln("error = ", error);
//   writeln("number of edges traversed = ", n_edges_traversed);


   if (error < 0){
     return error; 
   }
   else {
     return n_edges_traversed;
   }


}

proc compute_levels (ParentTree, Level): int
{
   use Graph500_defs;

   var error: int=0;

   forall k in vertex_domain with (ref error) do { // race

// Would prefer to be able to use a continue statement here as opposed to
// an if statement on these checks

     if ( error == 0 && ParentTree[k] > 0 && Level[k] < 0 ) {

       var parent: int = k;
       var nhop: int = 0;

       // Run up the tree until we encounter an already-leved vertex 

       while (parent >= 0 && Level[parent] < 0 && nhop < N_VERTICES) {
        nhop += 1; parent = ParentTree[parent];
       }

       if ( nhop > N_VERTICES) then error = -1;

       if (error >= 0 && parent < 0) then error = -2;

       // Assign levels until we meet an already-leveled vertex
   
       if ( error == 0 ) {
         nhop += Level[parent];
         parent = k;

         while ( Level[parent] < 0) {
           Level[parent] = nhop;
           nhop -= 1; 
           parent = ParentTree[parent];
         }
       }

     } 

   }
   return error;
}

}


