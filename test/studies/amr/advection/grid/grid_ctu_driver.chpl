//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use grid_base_defs;
use grid_solution_defs;
use grid_bc_defs;
use grid_ctu_defs;




//===> advanceAdvectionCTU routine ===>
//====================================>
def advanceAdvectionCTU(
  grid:           RectangularGrid,
  sol:            ScalarGridSolution,
  bc:             GridBC,
  velocity:       dimension*real,
  time_requested: real
){

  //==== Make sure the solution can validly be updated ====
  assert(sol.grid == grid);
  assert(sol.time(2) <= time_requested);


  //===> Calculate dt_target via CFL condition ===>
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  [d in dimensions] cfl(d) = grid.dx(d) / abs(velocity(d));
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
    bc.ghostFill(sol.space_data(2), sol.time(2));
    grid.stepCTU(sol, velocity, dt_used);
          
  }
  //<=== Time-stepping loop <===


}
//<=== advanceAdvectionCTU routine <===
//<====================================







def main {

  //===> Initialize output ===>
  var time_initial: real = 0.0,
    time_final:     real = 2.0,
    n_output:       int  = 20,
    output_times:   [1..n_output] real,
    dt_output:      real = (time_final - time_initial) / n_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = time_initial + i * dt_output;
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

  var grid = new RectangularGrid(x_low         = x_low,
                                 x_high        = x_high,
                                 i_low         = i_low,
                                 n_cells       = n_cells, 
                                 n_ghost_cells = n_ghost_cells);
  //<=== Initialize grid <===



  //==== Initialize boundary conditions ====
  var bc = new ZeroInflowGridBC(grid = grid);



  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*x(d)**2);
    return f;
  }

  var q = new ScalarMultiTimeGridSolution(grid    = grid,
                                          n_times = 2);
                                          
  grid.initializeSolution(q.time_layers(2), 
                          initial_condition,
                          time_initial);
  //<=== Initialize  solution <===






  //===> Generate output ===>
  //==== Initial time ====
  grid.clawOutput(q.time_layers(2), frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    advanceAdvectionCTU(grid, q, bc, velocity, output_time);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    grid.clawOutput(q.time_layers(2), frame_number);
  }
  //<=== Generate output <===
  

} // end main