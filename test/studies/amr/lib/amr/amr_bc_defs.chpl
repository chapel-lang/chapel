use amr_solution_defs;


//|"""""""""""""""""""\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {
  const hierarchy: AMRHierarchy;


  def fillGhostCells(
    q:             LevelArray, 
    sol_interface: LevelSolutionInterface,
    t:             real) 
  {

    //=== Interpolate fine boundary, if sol_interface is non-nil ====
    if sol_interface then
      q.fineBoundaryTimeInterpolation(interface, t);

    q.fillSharedGhosts();

    fillBoundaryGhosts(q, t);
  }


  def fillBoundaryGhosts(q: LevelArray, t: real) {}


}
// /"""""""""""""""""""|
//<=== AMRBC class <===|
// \___________________|