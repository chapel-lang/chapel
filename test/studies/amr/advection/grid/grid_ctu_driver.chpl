//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use grid_base_defs;
use grid_array_defs;
use grid_solution_defs;
use grid_bc_defs;
use grid_ctu_defs;


def main {

  //===> Initialize output ===>
  var initial_time: real = 0.0,
    final_time:     real = 2.0,
    n_output:       int  = 20,
    output_times:   [1..n_output] real,
    dt_output:      real = (final_time - initial_time) / n_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = initial_time + i * dt_output;
  //<=== Initialize output <===



  //==== Advection velocity ====
  //-------------------------------
  // velocity = (1, 3/4, 5/8, ...)
  //-------------------------------
  var velocity: dimension*real;
  [d in dimensions] velocity(d) = 0.5 + 0.5**d;



  //===> Initialize grid ===>
  var x_low, x_high:          dimension*real,
      i_low:                  dimension*int,
      n_cells, n_ghost_cells: dimension*int;

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
/*   var bc = new ZeroInflowAdvectionGridBC(grid = grid); */
  var bc = new PeriodicGridBC(grid = grid);



  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*x(d)**2);
    return f;
  }

  var sol = new GridSolution(grid = grid);

  sol.setToFunction(initial_condition, initial_time);
  //<=== Initialize solution <===




  //===> Generate output ===>
  //==== Initial time ====
  frame_number = 0;
  sol.clawOutput(frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    grid.advanceAdvectionCTU(sol, bc, velocity, output_time);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    sol.clawOutput(frame_number);
  }
  //<=== Generate output <===
  

} // end main





//===> advanceAdvectionCTU routine ===>
//====================================>
def BaseGrid.advanceAdvectionCTU(
  sol:            GridSolution,
  bc:             GridBC,
  velocity:       dimension*real,
  time_requested: real
){

  //==== Make sure the solution can validly be updated ====
  assert(sol.grid == this);
  assert(sol.time(2) <= time_requested);


  //===> Calculate dt_target via CFL condition ===>
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  [d in dimensions] cfl(d) = dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;
  //<=== Calculate dt_target via CFL condition <===
  

  
  //===> Time-stepping loop ===>
  while (sol.time(2) < time_requested) do {

    //==== Adjust the time step to hit time_requested if necessary ====
    if (sol.time(2) + dt_target > time_requested) then
      dt_used = time_requested - sol.time(2);
    else
      dt_used = dt_target;
    writeln("Taking step of size dt=", dt_used, 
	    " to time ", sol.time(2)+dt_used, ".");


    //==== Update solution ====
    stepCTU(sol, bc, velocity, dt_used);
          
  }
  //<=== Time-stepping loop <===


}
//<=== advanceAdvectionCTU routine <===
//<====================================