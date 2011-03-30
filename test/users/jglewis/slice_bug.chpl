module slice_bug

{
  config const SCALE = 6;

  const n_vertices = 2**SCALE;

  const n_raw_edges = 8 * n_vertices;
    
  config const a = .55;
    
  config const b = .10;
  
  config const c = .10;
  
  config const d = .25;

  record directed_vertex_pair {
    var start = 1: int;
    var end   = 1: int; 
  }
    
  proc +(l: directed_vertex_pair, r: directed_vertex_pair) 
      return new directed_vertex_pair (l.start + r.start, l.end + r.end);


  // ============================
  // Quadrant selection algorithm
  // ============================

  proc assign_quadrant ( u: real, a: real, b: real, c: real, d : real, 
			bit : int ) : directed_vertex_pair
    {
      var start_inc = 0;
      var end_inc   = 0;
      var edge : directed_vertex_pair;

      edge.start = bit * start_inc;
      edge.end   = bit * end_inc;
      return ( edge );
    }


  // ====================================
  // Main RMAT Graph Generation Procedure
  // ====================================~

  proc main {
    const vertex_range = 1..n_vertices, 
          edge_range   = 1..n_raw_edges,
          expanded_edge_range = 1..n_raw_edges + 1;

    var   U_2D       : [1..5,edge_range] real,
          U_expanded : [expanded_edge_range] real,
          U_5x       : [edge_range] real,
          Edges      = [edge_range] new directed_vertex_pair ();

    var bit = 1 << SCALE;

    Edges += assign_quadrant ( U_2D(5,..), a, b, c, d, bit );
    Edges += assign_quadrant ( U_expanded (edge_range), a, b, c, d, bit );
    //Edges += assign_quadrant ( U_expanded (5.. by 5), a, b, c, d, bit );
    writeln ("hello, I compiled successfully!");
  }
}

