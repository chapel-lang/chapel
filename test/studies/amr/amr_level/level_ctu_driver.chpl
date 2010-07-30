//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use level_base_defs;
use level_ctu_defs;




//===> advanceAdvectionCTU routine ===>
//====================================>
def advanceAdvectionCTU(level:          BaseLevel,
			sol:            LevelSolution,
			bc:             LevelBC,
			velocity:       dimension*real,
                        time_requested: real
                       ) {

  //==== Make sure the solution can validly be updated ====
  assert(sol.level == level  &&  sol.time <= time_requested);


  //===> Initialize ===>
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  [d in dimensions] cfl(d) = level.dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;
  //<=== Initialize <===
  

  
  //===> Time-stepping loop ===>
  while (sol.time < time_requested) do {

    //==== Adjust the time step to hit time_requested if necessary ====
    if (sol.time + dt_target > time_requested) then
      dt_used = time_requested - sol.time;
    else
      dt_used = dt_target;
    writeln("Taking step of size dt=", dt_used, 
	    " to time ", sol.time+dt_used, ".");


    //==== Update solution ====
    level.stepCTU(sol, bc, velocity, dt_used);
          
  }
  //<=== Time-stepping loop <===


}
//<=== advanceAdvectionCTU routine <===
//<====================================







def main {

  //==== Advection velocity ====
  //-------------------------------
  // velocity = (1, 3/4, 5/8, ...)
  //-------------------------------
  var velocity: dimension*real;
  [d in dimensions] velocity(d) = 0.5 + 0.5**d;



  //===> Initialize level ===>
  var x_low, x_high: dimension*real,
    n_cells, n_ghost: dimension*int;


  [d in dimensions] {
    x_low(d)    = -1.0;
    x_high(d)   =  1.0;
    n_cells(d)  = 100;
    n_ghost(d)  = 1;
  }

  var level = new BaseLevel(x_low               = x_low,
		            x_high              = x_high,
			    n_cells             = n_cells,
			    n_child_ghost_cells = n_ghost);
  //<=== Initialize level <===



  //===> Initialize grids ===>
  //==== Grid 1 ====
  x_low(1) = -1.0;
  x_high(1) = 0.0;

  for d in [2..dimension] {
    x_low(d)  = -1.0;
    x_high(d) = 0.5;
  }

  level.addGrid(x_low, x_high);


  //==== Grid 2 ====
  x_low(1) = 0.0;
  x_high(1) = 1.0;

  for d in [2..dimension] {
    x_low(d) = -0.5;
    x_high(d) = 1.0;
  }

  level.addGrid(x_low, x_high);


  //==== Fix the level ====
  level.fix();
  //<=== Initialize grids <===



  //==== Set boundary conditions ====
  var bc = new ZeroOrderExtrapLevelBC(level = level);



  //===> Initialize solution ===>
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30 * (x(d) + 0.5)**2);
    return f;
  }

  var sol = new LevelSolution(level = level);
  level.initializeSolution(sol, initial_condition);
  //<=== Initialize  solution <===



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



  //===> Generate output ===>
  //==== Initial time ====
  sol.time = time_initial;
  level.clawOutput(sol, frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    advanceAdvectionCTU(level, sol, bc, velocity, output_time);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    level.clawOutput(sol, frame_number);
  }
  //<=== Generate output <===
  

} // end main