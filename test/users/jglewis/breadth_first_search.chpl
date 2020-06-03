module BFS

  {

  // =======================================================================
  // Breadth First Search for simple directed or undirected graphs
  //
  // breadth first search expansion from a designated root node, returning
  // the minimun distance from root to each other node in the same connected
  // connected component of the graph g and the diameter of the graph.
  // =======================================================================

  proc breadth_first_search ( root, n_nodes, adj, out min_dist, 
                             out max_min_dist, out max_width )

     { 

     var Node_Set: domain (1) = {1..n_nodes};
     
     type Nbr_List = sparse subdomain ( Node_Set );
//     type Nbr_List = domain (int);

     var Active_Set          : [1..2] Nbr_List;

     // the graph G will be specified by the number of nodes  n_nodes
     // and the adjacency structure adj, which will be of type        
     // Adjacency_Structure

     var current_level = 1, next_level = 2;

     var current_distance = 0;

     // ---------------------------------------------------------
     // initialize the minimum distance array and the first level
     // ---------------------------------------------------------

     min_dist         = -1;
     min_dist (root)  = 0;
     max_width        = 1;

     // Active_Set (current_level) = (root);
     Active_Set (current_level).clear();
     Active_Set (current_level).add (root);

     //  ---------------------------------------------------------------------
     // Current_Level is the most distant set of nodes that have been visited.
     // All nodes in the  Current_Level  have a shortest path back to the root
     // of length  current_distance. 
     // Next_Level  will be the set of currently unvisited neighbors of any 
     // nodes in the current level.  All nodes in  Next_Level wil have a 
     // shortest path to the root of length  current_distance + 1.
     //  ---------------------------------------------------------------------

     do
        {

        // --------------------------------------------------------------------
        // Expand the unvisited neighbor set for all nodes in the current level
        // --------------------------------------------------------------------

        Active_Set (next_level).clear ();
        current_distance += 1;

        [ node in Active_Set (current_level) ] 
            expand_nbr_set ( node, adj, min_dist, Active_Set (next_level),
                             current_distance );

        // ----------------------
        // Move to the next level
        // ----------------------

        current_level <=> next_level;
        max_width = max ( max_width, Active_Set (current_level).size );

        }
     while Active_Set (current_level).size != 0;

     max_min_dist = current_distance;

     }

   // =====================================
   // examine neighbor list of a given node
   // =====================================

   proc expand_nbr_set ( node, adj, min_dist, Next_Level, current_distance )

     {

     // ----------------------------------------------------------------
     // examine each neighbor of "node"; add any yet unvisited neighbors
     // to the next level
     // ----------------------------------------------------------------

     [ nbr in adj (node) ]

       atomic 
          { if ( min_dist (nbr) < 0 )

	    // found a new, unvisited, neighbor

            { 
            Next_Level.add (nbr);
            min_dist (nbr) = current_distance;
            }

          }

     }

  }
          

