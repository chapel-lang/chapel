// =============================================================================
// =============================================================================
//
// Test Chapel Breadth First Search module with randomly generated graph
//
// =============================================================================
// =============================================================================

use Random, Time;

use BFS;

// default graph characterization

config const n_nodes = 100000, max_edges = 10;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 7402943902;



proc main {}
   {

   type Nbr_List = domain (int);

   var Node_Set            : domain (1) = {1..n_nodes};


   type Adjacency_Structure = [Node_Set] Nbr_List;

   var n_nbrs, min_dist     : [Node_Set] int;

   var adj                  : Adjacency_Structure;

   var randlist = new RandomStream (seed);

   var root = 1, max_min_dist = -1, max_width = -1;

   // -----------------------------------------------------------
   // Randomly generate the number of edges assigned to each node
   // -----------------------------------------------------------

   for node in Node_Set do
       n_nbrs (node) = (randlist.getNext() * max_edges):int + 1 ;

   writeln (" number of neighbors list generated ");

   // -----------------------------------------
   // Randomly generate neighbors for each node
   // -----------------------------------------

   forall node in Node_Set do

      for 1..n_nbrs (node) do

         adj (node) . add ( (randlist.getNext() * n_nodes):int + 1 );

   writeln (" neighbors list generated ");

   // ------------------------------------
   // Breadth first search from first node
   // ------------------------------------

   breadth_first_search ( root, n_nodes, adj, min_dist, max_min_dist, 
                          max_width );

   }


     


