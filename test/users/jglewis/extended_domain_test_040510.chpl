module SSCA2_main

{
  config const SCALE = 6;

  config param Torus_Base_Index = 1;

  proc main () {

    use BlockDist;

    const lg2d1 : int = SCALE / 2,
      lg2d2 = SCALE - lg2d1,
      d1    = 2**lg2d1,
      d2    = 2**lg2d2;

    const vertex_domain = { Torus_Base_Index..#d1,
			    Torus_Base_Index..#d2 }
      dmapped Block ( { Torus_Base_Index..#d1,
			Torus_Base_Index..#d2 } );

    type  vertex = index (vertex_domain);
    const extended_domain = vertex_domain.expand(1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    const h1 = vertex_domain.high (0), l1 = vertex_domain.low (0);
    const h2 = vertex_domain.high (1), l2 = vertex_domain.low (1);

    // torus_vertex wraps the torus with an additional plane in each dimension
    // fill in the wrap-around vertices in the ghost cells

    torus_vertex (l1-1, ..) = torus_vertex (h1 ,..);
    torus_vertex (h1+1, ..) = torus_vertex (l1,..);
    torus_vertex (.., l2-1) = torus_vertex (.., h2);
    torus_vertex (.., h2+1) = torus_vertex (.., l2);

    writeln ("succeeded in referencing extended domain without crashing");

   } // end of main

}
