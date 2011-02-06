use LevelBC_def;

//|\"""""""""""""""""""""""""""""""""""|\
//| >    ZeroInflowBC derived class    | >
//|/___________________________________|/
class ZeroInflowBC: LevelBC {

  proc apply(q: LevelVariable, t: real){
    apply_Homogeneous(q);
  }

  proc apply_Homogeneous(q: LevelVariable){

    for grid in level.grids {

      for ghost_domain in level.boundary(grid) {
        forall cell in ghost_domain do
          q(grid).value(cell) = 0.0;
      }

    }

  }
 
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    ZeroInflowBC derived class    < |
// \|___________________________________\|