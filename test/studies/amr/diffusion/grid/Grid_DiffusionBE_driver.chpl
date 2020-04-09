//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use GridVariable_DiffusionBE;
use GridSolution_DiffusionBE;
use GridBC_DiffusionBE;



proc main {

  //==== Output times ====
  var output_times = setOutputTimes("time.txt");


  //==== Grid ====
  var grid = readGrid("grid.txt");


  //==== Diffusivity ====
  const diffusivity = 0.1;


  //===> Initialize solution ===>
  proc initial_condition (x: dimension*real) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30*(x(d)+0.5)**2);
    return f;
  }

  var solution = new unmanaged GridSolution(grid);
  solution.setToFunction(initial_condition, output_times(0));

  var dt_max = 0.05;
  //<=== Initialize  solution <===



  //==== Initialize boundary conditions ====
  var bc = new unmanaged ZeroFluxDiffusionBC(grid = grid);
/*   var bc = new PeriodicGridBC(grid = grid); */



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  solution.clawOutput(frame_number);

  //==== Subsequent times ====
  for output_time in output_times(1..) do {
    //==== Advance q to output time ====
    solution.advance_DiffusionBE(bc, diffusivity, output_time, dt_max);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    solution.clawOutput(frame_number);
  }
  //<=== Generate output <===
  
  delete bc;
  delete solution;
  delete grid;
} // end main
