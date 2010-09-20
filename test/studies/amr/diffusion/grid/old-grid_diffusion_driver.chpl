//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use grid_solution_defs;
use grid_bc_defs;
use grid_diffusion_defs;




def main {

  //==== Output times ====
  var output_times = setOutputTimes("time.txt");


  //==== Grid ====
  var grid = readGrid("grid.txt");


  //==== Diffusivity ====
  const diffusivity = 0.1;


  //===> Initialize solution ===>
  def initial_condition (x: dimension*real) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30*(x(d)+0.5)**2);
    return f;
  }

  var sol = new GridSolution(grid);
  sol.setToFunction(initial_condition, output_times(1));

  var dt_max = 0.05;
  //<=== Initialize  solution <===



  //==== Initialize boundary conditions ====
  var bc = new ZeroFluxDiffusionGridBC(grid = grid);
/*   var bc = new PeriodicGridBC(grid = grid); */



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  sol.clawOutput(frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance q to output time ====
    grid.advanceDiffusionBE(sol, bc, diffusivity, output_time, dt_max);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    sol.clawOutput(frame_number);
  }
  //<=== Generate output <===
  

} // end main




//===> Grid.advanceDiffusionBE method ===>
//=======================================>
//----------------------------------------------------------------------
// Advances a GridSolution to the requested time, stepping forward with
// diffusion via Backward Euler.
//----------------------------------------------------------------------
def Grid.advanceDiffusionBE(
  sol:            GridSolution,
  bc:             GridBC,
  diffusivity:    real,
  time_requested: real,
  dt_max:         real)
{

  //==== Safety checks ====
  assert(sol.grid == this);
  assert(sol.time(2) <= time_requested);


  //===> Time-stepping ===>
  var n_steps = ceil( (time_requested - sol.time(2)) / dt_max ) : int;
  var dt_used = (time_requested - sol.time(2)) / n_steps;
   
   
  while (sol.time(2) < time_requested) {
    //==== Adjust the time step to hit time_requested if necessary ====
    if (sol.time(2) + dt_max > time_requested) then
      dt_used = time_requested - sol.time(2);
    else
      dt_used = dt_max;
    writeln("Taking step of size dt=", dt_used, " to time ", sol.time(2)+dt_used, ".");


    //==== Update solution ====
    stepDiffusionBE(sol, bc, diffusivity, dt_used, dt_used/4.0);

  }
  //<=== Time-stepping <===
 
}
//<=== Grid.advanceDiffusionBE method <===
//<=======================================