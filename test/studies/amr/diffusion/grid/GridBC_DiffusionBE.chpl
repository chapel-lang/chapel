use GridBC_def;


//|\"""""""""""""""""""""""""""""""""""|\
//| >    class: ZeroFluxDiffusionBC    | >
//|/___________________________________|/

class ZeroFluxDiffusionBC: GridBC {
  
  proc apply(q: GridVariable, t: real)
  {
    apply_Homogeneous(q);
  }


  proc apply_Homogeneous(q: GridVariable) {

    for ghost_domain in grid.ghost_domains {
      var loc = grid.relativeLocation(ghost_domain);
      var shift = -1*loc;

      forall cell in ghost_domain do
        q.value(cell) = q.value(cell+shift);

    }
    
  }
  
}

// /|"""""""""""""""""""""""""""""""""""/|
//< |    class: ZeroFluxDiffusionBC    < |
// \|___________________________________\|