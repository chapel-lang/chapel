use amr_solution_defs;


//|~~~~~~~~~~~~~~~~~~~\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {

  const hierarchy: AMRHierarchy;



  def fillGhostCells(q: LevelArray, t: real) {

    if q.level != hierarchy.top_level {
      var coarse_interface = hierarchy.coarse_interfaces(q.level);
      coarse_interface.LinearBoundaryInterpolate...
    }

  }

}
// /~~~~~~~~~~~~~~~~~~~|
//<=== AMRBC class <===|
// \___________________|