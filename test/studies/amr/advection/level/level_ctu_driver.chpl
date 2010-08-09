//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use level_base_defs;
use level_bcs;
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


  //==== Used to check dimension with each input file ====
  var dim_in: int;



  //===> Advection velocity ===>
  var velocity: dimension*real;
  var phys_file = new file("set_problem/physics.txt", FileAccessMode.read);
  phys_file.open();
  phys_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of physics.txt must equal " + format("%i",dimension));
  phys_file.readln((...velocity));
  phys_file.close();
  //<=== Advection velocity <===



  //===> Initialize space ===>
  var x_low, x_high: dimension*real,
    n_cells, n_ghost: dimension*int;

  var space_file = new file("set_problem/space.txt", FileAccessMode.read);
  space_file.open();

  space_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of space.txt must equal " + format("%i",dimension));
  space_file.readln(); // empty line

  space_file.readln(); // skip number of levels
  space_file.readln(); // empty line
  
  space_file.readln((...x_low));
  space_file.readln((...x_high));
  space_file.readln((...n_cells));
  space_file.readln((...n_ghost));

  var level = new BaseLevel(x_low               = x_low,
                            x_high              = x_high,
                            n_cells             = n_cells,
                            n_child_ghost_cells = n_ghost);

  space_file.readln();
  write("Setting up grids...");

  var n_grids: int;
  space_file.readln(n_grids);
  
  for i_grid in [1..n_grids] {
    write(i_grid, "...");
    space_file.readln();
    space_file.readln((...x_low));
    space_file.readln((...x_high));
    level.addGrid(x_low, x_high);
  }
  
  space_file.close();

  level.fix();
  write("done.\n");
  //<=== Initialize grids <===



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroInflowLevelBC(level = level);
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
  var time_file = new file("set_problem/time.txt", FileAccessMode.read);
  time_file.open();
  
  var time_initial, time_final: real;
  var n_output: int;

  time_file.readln(time_initial);
  time_file.readln(time_final);
  time_file.readln(n_output);
  time_file.close();

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
  

  // //===> Check content of ghost_is_shared ===>
  // for g in level.child_grids {
  //   writeln("");
  //   writeln("x_low  = ", g.x_low);
  //   writeln("x_high = ", g.x_high);
  //   for d in dimensions {
  //     writeln("");
  //     for cell in g.low_ghost_cells(d) {
  //       writeln("  ", g.xValue(cell), ": ", g.ghost_is_shared.low(d).value(cell)); 
  //     }      
  //     writeln("");
  //     for cell in g.high_ghost_cells(d) {
  //       writeln("  ", g.xValue(cell), ": ", g.ghost_is_shared.high(d).value(cell)); 
  //     }      
  //   }
  // }
  // //<=== Check content of ghost_is_shared <===
  

} // end main