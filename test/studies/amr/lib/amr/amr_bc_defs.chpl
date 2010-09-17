use amr_solution_defs;


//|"""""""""""""""""""\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {
  const hierarchy: AMRHierarchy;


  def fillGhostCells(
    q:         LevelArray, 
    interface: LevelSolutionInterface,
    t:         real) 
  {
    q.interpolateCFBoundary(interface, t);
    q.fillSharedGhosts();
    fillBoundaryGhosts(q, t);
  }


  def fillBoundaryGhosts(q: LevelSolution, t: real) {}


}
// /"""""""""""""""""""|
//<=== AMRBC class <===|
// \___________________|