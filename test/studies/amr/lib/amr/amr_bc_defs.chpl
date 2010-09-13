use amr_solution_defs;


//|~~~~~~~~~~~~~~~~~~~\
//|===> AMRBC class ===>
//|___________________/
class AMRBC {

  const hierarchy: AMRHierarchy;



  def fillGhostCells(
    q:            LevelArray, 
    amr_solution: AMRSolution, 
    t:            real) 
  {

    var level = q.level;


    if level != hierarchy.top_level {
      var coarse_interface = hierarchy.coarse_interfaces(level);
      var coarse_level     = coarse_interface.coarse_level;
      var coarse_solution  = amr_solution(coarse_level);

    }


  }

}
// /~~~~~~~~~~~~~~~~~~~|
//<=== AMRBC class <===|
// \___________________|