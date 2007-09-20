// =============================================================================
// =============================================================================
//
// Test Chapel Breadth First Search module with randomly generated graph
//
// =============================================================================
// =============================================================================

use Random, Time;

//use BFS;

// default graph characterization

config const n_nodes = 100000, max_edges = 10;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 7402943902;



def main {
  writeln("Got to main");
  exit(0);

  type Nbr_List = domain (int);
  
  var Node_Set            : domain (1) = [1..n_nodes];
  

  type Adjacency_Structure = [Node_Set] Nbr_List;
  
  var adj                  : Adjacency_Structure;
}


     


