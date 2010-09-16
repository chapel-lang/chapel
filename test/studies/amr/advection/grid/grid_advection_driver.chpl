//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use grid_solution_defs;
use grid_bc_defs;
use grid_advection_defs;


def main {

  //==== Output times ====
  var output_times = setOutputTimes("time.txt");


  //==== Grid ====
  var grid = readGrid("grid.txt");


  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*x(d)**2);
    return f;
  }

  var sol = new GridSolution(grid = grid);

  sol.setToFunction(initial_condition, output_times(1));
  //<=== Initialize solution <===



  //==== Advection velocity ====
  //  var velocity = readVelocity("parameters.txt");
  var velocity: dimension*real;
  [d in dimensions] velocity(d) = 0.5 + 0.5**d;



  //==== Initialize boundary conditions ====
/*   var bc = new ZeroInflowAdvectionGridBC(grid = grid); */
  var bc = new PeriodicGridBC(grid = grid);



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
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





//===> Grid.advanceAdvectionCTU method ===>
//========================================>
def Grid.advanceAdvectionCTU(
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
    stepAdvectionCTU(sol, bc, velocity, dt_used);
          
  }
  //<=== Time-stepping loop <===


}
//<=== grid.advanceAdvectionCTU routine <===
//<=========================================