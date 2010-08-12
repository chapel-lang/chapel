//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use grid_base_defs;
use grid_solution_defs;
use grid_bc_defs;
use grid_diffusion_defs;



//===> Backward Euler/conjugate gradient advancement of a GridSolution ===>
//========================================================================>
def BaseGrid.advanceDiffusionBE(sol:            ScalarGridSolution,
                                       bc:             GridBC,
                                       diffusivity:    real,
                                       time_requested: real,
                                       dt_max:         real) {

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
    stepBE(sol, bc, diffusivity, dt_used, dt_used/4.0);

  }
  //<=== Time-stepping <===
 
}
//<=== Backward Euler/conjugate gradient advancement of a GridSolution <===
//<========================================================================




def main {


  //===> Initialize output ===>
  var initial_time: real = 0.0,
    final_time:     real = 1.0,
    num_output:     int  = 20,
    output_times:   [1..num_output] real,
    dt_output:      real = (final_time - initial_time) / num_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = initial_time + i * dt_output;
  //<=== Initialize output <===




  //==== Diffusivity ====
  const diffusivity = 0.1;


  //===> Initialize grid ===>
  //---------------------------------------------------
  // The "_init" on each variable name is unnecessary,
  // but I find the constructor call to be unsettling
  // without.
  //---------------------------------------------------
  var x_low, x_high:           dimension*real,
      i_low:                   dimension*int,
      n_cells, n_ghost_cells:  dimension*int;

  var N: int;
  if dimension<3 then N=100;
  else N = 50;

  for d in dimensions do {
    x_low(d)         = -1.0;
    x_high(d)        = 1.0;
    i_low(d)         = 0;
    n_cells(d)       = N;
    n_ghost_cells(d) = 2;
  }

  var grid = new BaseGrid(x_low         = x_low,
		          x_high        = x_high,
                          i_low         = i_low,
                          n_cells       = n_cells, 
                          n_ghost_cells = n_ghost_cells);
  //<=== Initialize grid <===



  //==== Initialize boundary conditions ====
  var bc = new PeriodicGridBC(grid);



  //===> Initialize solution ===>
  def initial_condition (x: dimension*real) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30*(x(d)+0.5)**2);
    return f;
  }

  var sol = new ScalarGridSolution(grid);
  grid.initializeSolution(sol, initial_condition, initial_time);
  //<=== Initialize  solution <===






  //===> Generate output ===>
  //==== Initial time ====
  grid.clawOutput(sol, frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance q to output time ====
    grid.advanceDiffusionBE(sol, bc, diffusivity, output_time, 0.05);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    grid.clawOutput(sol, frame_number);
  }
  //<=== Generate output <===
  

} // end main