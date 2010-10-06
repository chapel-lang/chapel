use CFLevelInterface_def;


//|\""""""""""""""""""""""""""|\
//| >    fillAsNew methods    | >
//|/__________________________|/
def LevelArray.fillAsNew(
  q_old:     LevelArray,
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{
  for grid in level.grids do
    this(grid).fillAsNew(q_old, q_coarse, interface);
}


def GridArray.fillAsNew(
  q_old:     LevelArray,
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{

  //==== Initialize unfilled domains ====
  var unfilled_domains = new DomainSet(dimension, true);
  unfilled_domains.add(grid.cells);


  //==== Fill from old LevelArray ====
  for old_grid in q_old.level {
    var intersection = grid.cells( old_grid.cells );
    if intersection.numIndices > 0 {
      value(intersection) = q_old(old_grid, intersection);
      unfilled_domain_set -= intersection;
    }
  }


  //==== Interpolate from coarse LevelArray ====
  for coarse_grid in interface.coarse_neighbors(grid) {
    var overlap_set = unfilled_domain_set( interface.refine(coarse_grid.cells) );
    for overlap in overlap_set {
      value(overlap) =  q_coarse(coarse_grid).interpolateToFine_Linear(overlap);
      unfilled_domain_set -= overlap;
    }
  }

}
// /|""""""""""""""""""""""""""/|
//< |    fillAsNew methods    < |
// \|__________________________\|