
use LevelSolution_AdvectionCTU;
use AMRHierarchy_def;
use AMRBC_def;



//|\""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.advance_AdvectionCTU    | >
//|/__________________________________________|/
def AMRHierarchy.advance_AdvectionCTU(
  bc:             AMRBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(time < time_requested,
         "error: AMRHierarchy.advance_AdvectionCTU\n" +
         "AMRHierarchy.time = " + format("%8.4E",time) + ", while time_requested = " +
         format("%8.4E",time_requested));

  
  var cfl:       dimension*real;
  var dt_target: real;
  var dt:        real;

  cfl       = levels(1).dx / abs(velocity);
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
      stepLevel_AdvectionCTU(1, bc, velocity, dt);

      time += dt;
    }


}
// /|""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.advance_AdvectionCTU    < |
// \|__________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.stepLevel_AdvectionCTU    | >
//|/____________________________________________|/
def AMRHierarchy.stepLevel_AdvectionCTU(
  i_level: int,
  bc:        AMRBC,
  velocity:  dimension*real,
  dt:        real)
{
  //|\'''''''''''''''''''''''''''''''''''''''''|\
  //| >    Step solution on the input level    | >
  //|/.........................................|/
  const level_solution = level_solutions(i_level);
  const t = level_solution.current_time;

  //==== Physical boundary conditions ====
  bc.apply(i_level, level_solution.current_data, t);

  //==== If level is fine, get fine boundary values ====
  if i_level > 1 then
    level_solution.current_data.getFineBoundaryValues(coarse_overlap_solutions(i_level), t);

  level_solution.step_AdvectionCTU(velocity, dt);
  regrid_counters(i_level) -= 1;
  // /|'''''''''''''''''''''''''''''''''''''''''/|
  //< |    Step solution on the input level    < |
  // \|.........................................\|



  //|\'''''''''''''''''''''''''''''''''''''''''''''''|\
  //| >    Step solution on the next-finest level    | >
  //|/...............................................|/
  if i_level < level_indices.high {

    //==== Fill fine boundary values for the next level ====
    coarse_overlap_solutions(i_level+1).fill_Linear(level_solution);

    //==== Step the finer solution as many times as necessary ====
    const ref_ratio = refinementRatio(levels(i_level), levels(i_level+1));
    const time_ref_ratio = max( (...ref_ratio) );
    const dt_fine = dt / time_ref_ratio;
    for i in 1..time_ref_ratio {
      stepLevel_AdvectionCTU(i_level+1, bc, velocity, dt_fine);
      // writeln("regrid_counters(", i_level+1, ") = ", regrid_counters(i_level+1));
      if regrid_counters(i_level+1)==0 then regrid(i_level);
    }
    //==== Correct the solution on the input level ====
    level_solution.correct_Linear(level_solutions(i_level+1), fine_boundaries(i_level));
  }
  // /|'''''''''''''''''''''''''''''''''''''''''''''''/|
  //< |    Step solution on the next-finest level    < |
  // \|...............................................\|

}
// /|""""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.stepLevel_AdvectionCTU    < |
// \|____________________________________________\|