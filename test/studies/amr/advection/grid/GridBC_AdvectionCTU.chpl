public use GridBC_def;



//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    derived class: ZeroInflowAdvectionBC    | >
//|/_____________________________________________|/

class ZeroInflowAdvectionBC: GridBC {
  
  //---- This type of BC is homogeneous ----
  
  override proc apply ( q: unmanaged GridVariable, t: real ) { apply_Homogeneous(q); }
  
  
  override proc apply_Homogeneous(q: unmanaged GridVariable) {

    for ghost_domain in grid.ghost_domains {
      forall cell in ghost_domain do
        q.value(cell) = 0.0;
    }
    
  }
  
}

// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    derived class: ZeroInflowAdvectionBC    < |
// \|_____________________________________________\|
