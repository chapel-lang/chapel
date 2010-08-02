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

  var f: file = new file("grids_in.txt", FileAccessMode.read);
  f.open();

  f.readln(); // skip number of levels
  f.readln(); // empty line
  
  f.readln((...x_low));
  f.readln((...x_high));
  f.readln((...n_cells));
  f.readln((...n_ghost));

  var level = new BaseLevel(x_low               = x_low,
                            x_high              = x_high,
                            n_cells             = n_cells,
                            n_child_ghost_cells = n_ghost);

  f.readln();
  write("Setting up grids...");

  var n_grids: int;
  f.readln(n_grids);
  
  for i_grid in [1..n_grids] {
    write(i_grid, "...");
    f.readln();
    f.readln((...x_low));
    f.readln((...x_high));
    level.addGrid(x_low, x_high);
  }
  
  f.close();

  level.fix();
  write("done.\n");
  //<=== Initialize grids <===



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroOrderExtrapLevelBC(level = level);
  write("done.\n");


  //===> Initialize solution ===>
  write("Initializing solution...");
  def initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30 * (x(d) + 0.5)**2);
    return f;
  }

  var sol = new LevelSolution(level = level);
  level.initializeSolution(sol, initial_condition);
  write("done.\n");
  //<=== Initialize  solution <===



  //===> Initialize output ===>
  f = new file("time_parameters.txt", FileAccessMode.read);
  f.open();
  
  var time_initial, time_final: real;
  var n_output: int;

  f.readln(time_initial);
  f.readln(time_final);
  f.readln(n_output);
  f.close();

  var output_times:   [1..n_output] real,
    dt_output:      real = (time_final - time_initial) / n_output,
    frame_number:   int = 0;

  for i in output_times.domain do
    output_times(i) = time_initial + i * dt_output;
  //<=== Initialize output <===



  //===> Generate output ===>
  //==== Initial time ====
  write("Writing initial output...");
  sol.time = time_initial;
  level.clawOutput(sol, frame_number);
  write("done.\n");

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    advanceAdvectionCTU(level, sol, bc, velocity, output_time);

    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    level.clawOutput(sol, frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  

} // end main