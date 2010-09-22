use LevelBC_def;

//|\"""""""""""""""""""""""""""""""""""|\
//| >    ZeroInflowBC derived class    | >
//|/___________________________________|/
class ZeroInflowBC: LevelBC {

  def apply(q: LevelArray, t: real){
    apply_Homogeneous(q);
  }

  def apply_Homogeneous(q: LevelArray){

    for grid in level.grids {

      for ghost_domain in grid.ghost_domain_set {
        forall cell in ghost_domain do
          q(grid).value(cell) = 0.0;
      }

    }

  }
 
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    ZeroInflowBC derived class    < |
// \|___________________________________\|