
use LevelSolution_AdvectionCTU;
use StaticMRHierarchy_def;
use StaticMRSolution_def;
use StaticMRBC_def;



//|\""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRSolution.advance_AdvectionCTU    | >
//|/______________________________________________|/
proc StaticMRSolution.advance_AdvectionCTU(
  bc:             StaticMRBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(time < time_requested,
         "error: AMRSolution.advance_AdvectionCTU\n" +
         "AMRSolution.time = " + format("%8.4E",time) + ", while time_requested = " +
         format("%8.4E",time_requested));

  
  var cfl:       dimension*real;
  var dt_target: real;
  var dt:        real;

  cfl       = hierarchy.levels(1).dx / abs(velocity);
  dt_target = min( (...cfl) );
  dt_target *= 0.95;


  while time < time_requested {
      //==== Choose time step ====
      if time + dt_target > time_requested then
        dt = time_requested - time;
      else
        dt = dt_target;
      writeln("Taking step of size dt=", dt, " to time ", time+dt, ".");


      //==== Step solution forwards ====
      //---------------------------------------------------------------------
      // This call invokes the same method on all levels down the hierarchy,
      // so stepping the top level is all that is needed.
      //---------------------------------------------------------------------
      stepLevelSolution_AdvectionCTU(1, bc, velocity, dt);

    }


}
// /|""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRSolution.advance_AdvectionCTU    < |
// \|______________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRSolution.stepLevelSolution_AdvectionCTU    | >
//|/________________________________________________________|/
proc StaticMRSolution.stepLevelSolution_AdvectionCTU(
  level_idx: int,
  bc:        StaticMRBC,
  velocity:  dimension*real,
  dt:        real)
{
  //|\'''''''''''''''''''''''''''''''''''''''''|\
  //| >    Step solution on the input level    | >
  //|/.........................................|/
  const level_solution = level_solutions(level_idx);
  const t = level_solution.current_time;

  //==== Physical boundary conditions ====
  bc.apply(level_idx, level_solution.current_data, t);

  //==== If level is fine, get fine boundary values ====
  if level_idx > 1 then
    level_solution.current_data.getFineBoundaryValues(coarse_overlap_solutions(level_idx), t);

  level_solution.step_AdvectionCTU(velocity, dt);
  // /|'''''''''''''''''''''''''''''''''''''''''/|
  //< |    Step solution on the input level    < |
  // \|.........................................\|



  //|\'''''''''''''''''''''''''''''''''''''''''''''''|\
  //| >    Step solution on the next-finest level    | >
  //|/...............................................|/
  if level_idx < hierarchy.level_indices.high {

    //==== Fill fine boundary values for the next level ====
    coarse_overlap_solutions(level_idx+1).fill_Linear(level_solution);

    //==== Step the finer solution as many times as necessary ====
    const ref_ratio = refinementRatio(hierarchy.levels(level_idx), hierarchy.levels(level_idx+1));
    const time_ref_ratio = max( (...ref_ratio) );
    const dt_fine = dt / time_ref_ratio;
    for i in 1..time_ref_ratio do
      stepLevelSolution_AdvectionCTU(level_idx+1, bc, velocity, dt_fine);

    //==== Correct the solution on the input level ====
    level_solution.correct_Linear(level_solutions(level_idx+1), hierarchy.fine_boundaries(level_idx));
  }
  // /|'''''''''''''''''''''''''''''''''''''''''''''''/|
  //< |    Step solution on the next-finest level    < |
  // \|...............................................\|

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRSolution.stepLevelSolution_AdvectionCTU    < |
// \|________________________________________________________\|