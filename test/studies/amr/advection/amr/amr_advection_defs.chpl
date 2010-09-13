//===> Description ===>
//
// Defines an advection/CTU solver on an AMRHierarchy.
//
//<=== Description <===

use amr_base_defs;
use amr_solution_defs;





def AMRSolution.levelStepAdvectionCTU(
  level:    BaseLevel,
  bc:       AMRBC,
  velocity: dimension*real,
  dt:       real)
{
  
  //.... How will this access a coarser level for boundary info?
  // The BC does know about the hierarchy, so I suppose that'll work.
  // I guess the better question is, how will an AMRBC work?
  level_solutions(level).stepAdvectionCTU(bc, velocity, dt);

  var level = level_solution.level;

  if level != hierarchy.bottom_level {
    var fine_interface = hierarchy.fine_interface(level);
    var fine_solution = level_solutions(fine_interface.fine_level);
    var time_ref_ratio = max( ...fine_interface.ref_ratio );

    for i in [1..time_ref_ratio] do
      AMRSolution.levelStepAdvectionCTU(fine_solution, bc,
					velocity, dt/time_ref_ratio);
  }    
  

}



def LevelSolution.stepAdvectionCTU(
  bc:       AMRBC,
  velocity: dimension*real,
  dt:       real)
{


}






}