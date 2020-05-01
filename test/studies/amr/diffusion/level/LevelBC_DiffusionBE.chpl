public use LevelBC_def;


//|\""""""""""""""""""""""""""""""""""""""""""|\
//| >    ZeroFluxDiffusionBC derived class    | >
//|/__________________________________________|/
class ZeroFluxDiffusionBC: LevelBC {
  
  override proc apply(q: unmanaged LevelVariable, t: real) {
    apply_Homogeneous(q);
  }

  
  override proc apply_Homogeneous(q: unmanaged LevelVariable) {
    
    for grid in level.grids {
    
      for ghost_domain in grid.ghost_domains {
        var loc   = grid.relativeLocation(ghost_domain);
        var shift = -1*loc;
        
        forall cell in ghost_domain do
          q(grid)!.value(cell) = q(grid)!.value(cell+shift);
      }
    
    } // end for grid in level.grids
  }

}
// /|""""""""""""""""""""""""""""""""""""""""""/|
//< |    ZeroFluxDiffusionBC derived class    < |
// \|__________________________________________\|
