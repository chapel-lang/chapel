//===> Description ===>
//
// Driver for a diffusion example, integrated with Backward Euler
//
//<=== Description <===


use LevelSolution_DiffusionBE;
use LevelBC_DiffusionBE;

use IO;

proc main {


  //==== Output times ====
  var output_times = setOutputTimes("set_problem/time.txt");


  //==== Level ====
  var level = readLevel("set_problem/space.txt");



  //===> Initialize solution ===>
  write("Initializing solution...");
  proc initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30 * (x(d) + 0.5)**2);
    return f;
  }

  var solution = new unmanaged LevelSolution(level = level);
  solution.setToFunction(initial_condition, output_times(0));

  //==== Max time step ====
  //--------------------------------------------------------------------
  // Once I can make the solution a derived class, this should probably
  // be a field.
  //--------------------------------------------------------------------
  var dt_max = 0.05;

  write("done.\n");
  //<=== Initialize  solution <===



  //===> Diffusivity ===>
  var diffusivity: real;
  var phys_file = open("set_problem/physics.txt", iomode.r).reader();
  var dim_in: int;
  phys_file.readln(diffusivity);
  phys_file.close();
  //<=== Diffusivity <===



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new unmanaged ZeroFluxDiffusionBC(level = level);
  write("done.\n");



  //===> Generate output ===>
  //==== Initial time ====
  write("Writing initial output...");
  var frame_number: int = 0;
  solution.clawOutput(frame_number);
  write("done.\n");
  
  //==== Subsequent times ====
  for output_time in output_times(1..) do {
    //==== Advance solution to output time ====
    solution.advance_DiffusionBE(bc, diffusivity, output_time, dt_max);
  
    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    solution.clawOutput(frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  
  delete bc;
  delete solution;
  delete level;
} // end main
