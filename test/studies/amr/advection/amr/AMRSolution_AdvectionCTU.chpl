//===> Description ===>
//
// Defines an advection/CTU solver on an AMRHierarchy.
//
//<=== Description <===

use LevelSolution_AdvectionCTU;
use AMRHierarchy_def;
use AMRSolution_def;
use AMRBC_def;



//|""""""""""""""""""""""""""""""""""""""""\
//|===> AMRSolution.advance_AdvectionCTU ===>
//|________________________________________/
def AMRSolution.advance_AdvectionCTU(
  bc:             AMRBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(time < time_requested,
         "error: AMRSolution.advance_AdvectionCTU\n" +
         "AMRSolution.time = " + format("%8.4E",time) + ", while time_requested = " +
         format("%8.4E",time_requested));

  
  var cfl: [dimensions] real;
  var dt_target: real;
  var dt: real;

  for d in dimensions do cfl(d) = hierarchy.top_level.dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
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
      stepLevelSolution_AdvectionCTU(hierarchy.top_level, bc, velocity, dt);

    }


}
// /""""""""""""""""""""""""""""""""""""""""/
//<=== AMRSolution.advance_AdvectionCTU <==<
// \________________________________________\






//|""""""""""""""""""""""""""""""""""""""""""""""""""\
//|===> AMRSolution.stepLevelSolution_AdvectionCTU ===>
//|__________________________________________________/
def AMRSolution.stepLevelSolution_AdvectionCTU(
  level:    Level,
  bc:       AMRBC,
  velocity: dimension*real,
  dt:       real)
{
  //===> Step the solution on the input level ===>
  var level_solution = level_solutions(level);
  var coarse_sol_interface = coarse_interfaces(level);
  var t = level_solution.current_time;

  //---- Fill ghost cells ----
  bc.apply(level_solution.current_data, t);
  if coarse_sol_interface then
    level_solution.current_data.fineBoundaryTimeInterpolation(coarse_sol_interface, t);

  level_solutions(level).step_AdvectionCTU(velocity, dt);
  //<=== Step the solution on the input level


  //===> Step the next level down ===>
  if level != hierarchy.bottom_level {

    //==== Identify fine interfaces ====
    var fine_level = hierarchy.finerLevel(level);
    var fine_level_interface = hierarchy.fine_interfaces(level);
    var fine_solution_interface = fine_interfaces(level);


    //==== Fill the solution interface's fine boundary ====
    fine_solution_interface.fillFineBoundaryData_Linear();


    //==== Step the finer solution as many times as necessary ====
    var time_ref_ratio = max( (...fine_level_interface.ref_ratio) );
    var dt_fine = dt / time_ref_ratio;
    for i in 1..time_ref_ratio do
      stepLevelSolution_AdvectionCTU(fine_level, bc, velocity, dt_fine);


    //==== Correct the solution on the input level ====
    fine_solution_interface.correctCoarseSolution_Linear();
  }
  //<=== Step the next level down <===

}
// /""""""""""""""""""""""""""""""""""""""""""""""""""/
//<=== AMRSolution.stepLevelSolution_AdvectionCTU <==<
// \__________________________________________________\