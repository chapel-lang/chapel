//===> Description ===>
//
// Driver for a diffusion example, integrated with Backward Euler
//
//<=== Description <===


use level_base_defs;
use level_solution_defs;
use level_bc_defs;
use level_diffusion_defs;



//===> Level.advanceDiffusionBE method ===>
//========================================>
def Level.advanceDiffusionBE(sol:            LevelSolution,
                                 bc:             LevelBC,
                                 diffusivity:    real,
                                 time_requested: real,
                                 dt_max:         real) {

  //==== Safety checks ====
  assert(sol.level == this);
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
//<=== Level.advanceDiffusionBE method <===
//<========================================



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

  var dt_max = 0.05;
  //<=== Initialize output <===



  //===> Diffusivity ===>
  var diffusivity: real;
  var phys_file = new file("set_problem/physics.txt", FileAccessMode.read);
  phys_file.open();
  var dim_in: int;
  phys_file.readln(diffusivity);
  phys_file.close();
  //<=== Diffusivity <===



  //===> Initialize space ===>
  var level = levelFromInputFile("set_problem/space.txt");



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroFluxDiffusionLevelBC(level = level);
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
  write("Writing initial output...");
  frame_number = 0;
  sol.clawOutput(frame_number);
  write("done.\n");
  
  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance solution to output time ====
    level.advanceDiffusionBE(sol, bc, diffusivity, output_time, dt_max);
  
    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    sol.clawOutput(frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  
  

} // end main