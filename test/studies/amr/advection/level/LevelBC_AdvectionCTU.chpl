public use LevelBC_def;

//|\"""""""""""""""""""""""""""""""""""|\
//| >    ZeroInflowBC derived class    | >
//|/___________________________________|/
class ZeroInflowBC: LevelBC {

  override proc apply(q: unmanaged LevelVariable, t: real){
    apply_Homogeneous(q);
  }

  override proc apply_Homogeneous(q: unmanaged LevelVariable){

    for grid in level.grids {

      for ghost_domain in level.boundary(grid)! {
        forall cell in ghost_domain do
          q(grid)!.value(cell) = 0.0;
      }

    }

  }
 
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    ZeroInflowBC derived class    < |
// \|___________________________________\|
