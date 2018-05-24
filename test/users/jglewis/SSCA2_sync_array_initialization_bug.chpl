module SSCA2_sync_array_initialization_bug

{
  config const SCALE = 4;
  config param Torus_Base_Index = 1;

  use BlockDist;

  class torus_fully_implicit_stencil_graph {
    const vertex_domain;
    const vertices = vertex_domain;
  }

  proc main () {

    const d = 2**SCALE;
  
    const vertex_domain = { Torus_Base_Index..#d }
      dmapped Block ( {Torus_Base_Index..#d} ); 
  
    var G = new unmanaged torus_fully_implicit_stencil_graph ( vertex_domain );
    var good_initialization : [G.vertices] bool;

    writeln ( "min_distance arrays SHOULD be all -1's except for a solo 0");
      
    forall s in G.vertices do {
  
      var min_distance$ : [G.vertices] sync int = -1;
  
      min_distance$ (s) . writeFF (0);
  
      good_initialization (s) = & reduce [v in G.vertices ] 
	                          if v == s then 
	                             min_distance$(s).readFF () == 0
				   else
				     min_distance$(v).readFF () == -1;  
  
    }; // closure of outer embarassingly parallel forall

    if & reduce [v in G.vertices ] good_initialization (v) then
      writeln ("success");
    else
      writeln ("failure");

    delete G;
  
  } // end of main

}
