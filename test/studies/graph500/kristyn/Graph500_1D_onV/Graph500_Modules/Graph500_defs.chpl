module Graph500_defs
{

  public use BlockDist;
  config param DISTRIBUTION_TYPE = "BLOCK";
  config param REPRODUCIBLE_PROBLEMS = true;
  config param PRINT_TIMING_STATISTICS = true;
  config param PRINT_LOCALE_INFORMATION = true;
  config param RMAT_WITH_NOISE = true;
  config param RMAT_WITH_SHUFFLE = true;
  config param SHUFFLE_DETERMINISTICALLY = false;
  config param DEBUG_GRAPH_GENERATOR = false;
  config const SCALE = 6;
  config const EDGEFACTOR = 8;
  const N_VERTICES = 1 << SCALE;
  const N_RAWEDGES = N_VERTICES*EDGEFACTOR;
  const NUMROOTS = 4;
  const NUM_CANDIDATES = 1024;


// The data structure used to store the edges is an array of records

  const edgelist_domain =
    {1..N_RAWEDGES} dmapped Block ( {1..N_RAWEDGES} );

  record directed_vertex_pair {
    var start = 1: int;
    var end   = 1: int;
  }

// Here we have overloaded the + operator
  proc +(l: directed_vertex_pair, r: directed_vertex_pair)
      return new directed_vertex_pair (l.start + r.start, l.end + r.end);

// The data structures below are chosen with the intention of later defining
// a user distribution which can implement a loop collapse of the Iterator over
// vertices in the Level_Set and the neighbor lists of these vertices.
// This is accomplished using 1 dimensional domains.
//

    const vertex_domain =
      if DISTRIBUTION_TYPE == "BLOCK" then
        {1..N_VERTICES} dmapped Block ( {1..N_VERTICES} )
      else
        {1..N_VERTICES} ;

    type vertex_id = int;

    record vertex_struct {
      var nd: domain(1);
      var Neighbors : [nd] vertex_id;
      var neighbor_count: int=0;
      var self_edges: int=0;
      var duplicates: int=0;
      var vlock$: sync bool = true;

      proc is_a_neighbor (new_vertex_ID: vertex_id) {
         var is_member: bool = false;
         forall n in Neighbors (1..neighbor_count) with (ref is_member) {
            if (new_vertex_ID == n ) then is_member = true;
         }
         return is_member;
      }
        
      proc add_self_edge () {
         vlock$.readFE();
         self_edges += 1;
         vlock$.writeEF(true);
      }
 
      proc add_duplicate () {
         vlock$.readFE();
         duplicates += 1;
         vlock$.writeEF(true);
      }

      proc add_Neighbor (new_vertex_ID: vertex_id) {
         vlock$.readFE();
         var ID: vertex_id = new_vertex_ID;
//       Check again to make sure another thread did not recently
//       add v to u's neighbor list
         if is_a_neighbor(ID) then {
           duplicates +=1;
         }
         else
         {
           if (neighbor_count >= Neighbors.size) {
             grow_helper(); 
           }
           neighbor_count += 1;
           Neighbors[neighbor_count]= new_vertex_ID;
         }
         vlock$.writeEF(true);
      }
         
      proc grow_helper() { 
          halt("Should not call grow_helper");
          var new_nd = Neighbors.size + 1;
          nd = {1..new_nd};
      }

    }

    class Graph {
      const my_vertices;
      var   Vertices : [my_vertices] vertex_struct;

      proc   Neighbors ( v : vertex_id ) {return Vertices (v).Neighbors;}

//      proc   n_Neighbors (v : vertex_id )
//      {return Vertices(v).neighbor_count;}


    }


// Data structures needed for BFS

// SSCA2 code uses level sets. The set of vertices at a particular distance
// from the starting vertex form a level set
// The class allows the full set of vertices visited to be partitioned into a
// linked list of level sets 

class Level_Set {
  type Vertex_List;
  var Members: Vertex_List;
  var previous: unmanaged Level_Set (Vertex_List)?;
}

 public use ReplicatedDist;


}
