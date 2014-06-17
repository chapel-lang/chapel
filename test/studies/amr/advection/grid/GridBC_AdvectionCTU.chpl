use GridBC_def;



//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    derived class: ZeroInflowAdvectionBC    | >
//|/_____________________________________________|/

class ZeroInflowAdvectionBC: GridBC {
  
  //---- This type of BC is homogeneous ----
  
  proc apply ( q: GridVariable, t: real ) { apply_Homogeneous(q); }
  
  
  proc apply_Homogeneous(q: GridVariable) {

    for ghost_domain in grid.ghost_domains {
      forall cell in ghost_domain do
        q.value(cell) = 0.0;
    }
    
  }
  
}

// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    derived class: ZeroInflowAdvectionBC    < |
// \|_____________________________________________\|