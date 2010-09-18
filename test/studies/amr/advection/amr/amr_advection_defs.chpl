//===> Description ===>
//
// Defines an advection/CTU solver on an AMRHierarchy.
//
//<=== Description <===

use amr_base_defs;
use amr_solution_defs;




//|""""""""""""""""""""""""""""""""""""""""\
//|===> AMRSolution.advance_AdvectionCTU ===>
//|________________________________________/
def AMRSolution.advance_AdvectionCTU(
  bc:             AMRBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(time < time_requested);

  
  var cfl: [dimensions] real;
  var dt_target: real;
  var dt: real;

  for d in dimensions do cfl(d) = dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;


  while time < time_requested {
      //==== Choose time step ====
      if time + dt_target > time_requested then
	dt = time_requested - time;
      else
	dt = dt_target;
      writeln("Taking step of side dt=", dt, " to time ", time+dt, ".");


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
  level:    BaseLevel,
  bc:       AMRBC,
  velocity: dimension*real,
  dt:       real)
{
  
  //==== Step the solution on the input level ====
  var level_solution = level_solutions(level);
  var coarse_sol_interface = coarse_interfaces(level);
  var t = level_solution.time(1);

  if coarse_sol_interface then
    level_solution.space_data(1).interpolateBoundaryData(coarse_sol_interface, t);

  bc.fillPhysicalBoundary(level_solution.space_data(1), t);
  level_solutions(level).step_AdvectionCTU(velocity, dt);


  //===> Step the next level down ===>
  if level != hierarchy.bottom_level {

    //==== Identify fine interfaces ====
    var fine_level = hierarchy.finerLevel(level);
    var fine_lev_interface = hierarchy.fine_interfaces(level);
    var fine_sol_interface = fine_interfaces(level);


    //==== Fill the solution interface's fine boundary ====
    fine_sol_interface.fillFineBoundary_Linear();


    //==== Step the finer solution as many times as necessary ====
    var time_ref_ratio = max( (...fine_lev_interface.ref_ratio) );
    var dt_fine = dt / time_ref_ratio;
    for i in [1..time_ref_ratio] do
      AMRSolution.levelStepAdvectionCTU(fine_level, bc, velocity, dt_fine);


    //==== Correct the solution on the input level ====
    fine_sol_interface.correctCoarseSolution_Linear();
  }
  //<=== Step the next level down <===

}
// /""""""""""""""""""""""""""""""""""""""""""""""""""/
//<=== AMRSolution.stepLevelSolution_AdvectionCTU <==<
// \__________________________________________________\




//|"""""""""""""""""""""""""""""""""""""""\
//|===> LevelSolution.step_AdvectionCTU ===>
//|_______________________________________/
def LevelSolution.step_AdvectionCTU(
  velocity:                  dimension*real,
  dt:                        real)
{


  //==== Step each grid ====
  for grid in grids do
    grid.step_AdvectionCTU(q_new(grid), q_current(grid), velocity, dt);


  //==== Update solution structure ====
  level_sol.space_data(1) = q_current;
  level_sol.space_data(2) = q_new;
  level_sol.time(1) = t_current;
  level_sol.time(2) = t_new;


}
// /"""""""""""""""""""""""""""""""""""""""/
//<=== LevelSolution.step_AdvectionCTU <==<
// \_______________________________________\






//===> GridSolution.step_AdvectionCTU method ===>
//=====================================>
def Grid.stepAdvectionCTU(
  q_new:     GridArray,
  q_current: GridArray,
  velocity:  dimension*real,
  dt:        real)
{

  //==== Safety check ====
  assert(q_new.grid == this);
  assert(q_current.grid == this);



  var alignments: domain(dimension);
  alignments = alignments.expand(1);
  
  //===> Calculate new value ===>
  forall precell in cells {

    //==== 1D/tuple fix ====
    var cell = tuplify(precell);


    q_new.value(cell) = 0.0;
    var volume_fraction: real;
    var upwind_cell: dimension*int;


    //===> Update for each alignment ===>
    for prealignment in alignments {

      //==== 1D/tuple fix ====
      var alignment = tuplify(prealignment);

        
      volume_fraction = 1.0;
      for d in dimensions {
        //-------------------------------------------------------
        // For each alignment, set the volume fraction and index
        // of the upwind cell. 
        //-------------------------------------------------------
        if alignment(d) == 0 then {
          volume_fraction *= abs(velocity(d))*dt / dx(d);
        if velocity(d) < 0.0 then
          upwind_cell(d) = cell(d)+2;
        else // the case velocity(d)==0 can refer to any cell
          upwind_cell(d) = cell(d)-2;
	      }
	      else {
          volume_fraction *= 1.0 - abs(velocity(d))*dt / dx(d);
          upwind_cell(d) = cell(d);
        }
          
      }

          
      //==== Update cell value ====
      q_new.value(cell) += volume_fraction * q_current.value(upwind_cell);
  
    }
    //<=== Update for each alignment <===
    
  }
  //<=== Calculate new value <===


}






}