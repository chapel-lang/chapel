//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
//<=== Description <===


use level_solution_defs;
use level_bc_defs;
use level_advection_defs;



def main {


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



  //===> Advection velocity ===>
  var velocity: dimension*real;
  var phys_file = new file("set_problem/physics.txt", FileAccessMode.read);
  phys_file.open();
  var dim_in: int;
  phys_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of physics.txt must equal " + format("%i",dimension));
  phys_file.readln((...velocity));
  phys_file.close();
  //<=== Advection velocity <===



  //===> Initialize space ===>  
  var level = levelFromInputFile("set_problem/space.txt");



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroInflowAdvectionLevelBC(level = level);
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
  sol.setToFunction(initial_condition, time_initial);
  write("done.\n");
  //<=== Initialize  solution <===




  //===> Generate output ===>
  //==== Initial time ====
  frame_number = 0;
  write("Writing initial output...");
  sol.clawOutput(frame_number);
  write("done.\n");
  
  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    level.advanceAdvectionCTU(sol, bc, velocity, output_time);
  
    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    sol.clawOutput(frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  
  

} // end main





//===> Level.advanceAdvectionCTU method ===>
//=========================================>
def Level.advanceAdvectionCTU(
  sol:            LevelSolution,
  bc:             LevelBC,
  velocity:       dimension*real,
  time_requested: real)
{

  //==== Safety check ====
  assert(sol.level == this);
  assert(sol.time(2) <= time_requested);


  //===> Initialize ===>
  var cfl: [dimensions] real,
      dt_target:        real,
      dt_used:          real;

  [d in dimensions] cfl(d) = dx(d) / abs(velocity(d));
  (dt_target,) = minloc reduce(cfl, dimensions);
  dt_target *= 0.95;
  //<=== Initialize <===
  

  
  //===> Time-stepping loop ===>
  while (sol.time(2) < time_requested) do {

    //==== Adjust the time step to hit time_requested if necessary ====
    if (sol.time(2) + dt_target > time_requested) then
      dt_used = time_requested - sol.time(2);
    else
      dt_used = dt_target;
    writeln("Taking step of size dt=", dt_used,
	    " to time ", sol.time(2) + dt_used, ".");


    //==== Update solution ====
    stepAdvectionCTU(sol, bc, velocity, dt_used);
          
  }
  //<=== Time-stepping loop <===


}
//<=== Level.advanceAdvectionCTU method <===
//<=========================================