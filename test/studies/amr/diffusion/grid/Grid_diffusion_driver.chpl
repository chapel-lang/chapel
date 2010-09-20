//===> Description ===>
//
// Driver for an diffusion example, integrated with Backward Euler.
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use GridArray_diffusion;
use GridSolution_diffusion;
use GridBC_diffusion;



def main {

  //==== Output times ====
  var output_times = setOutputTimes("time.txt");


  //==== Grid ====
  var grid = readGrid("grid.txt");


  //==== Diffusivity ====
  const diffusivity = 0.1;


  //===> Initialize solution ===>
  def initial_condition (x: dimension*real) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30*(x(d)+0.5)**2);
    return f;
  }

  var solution = new GridSolution(grid);
  solution.setToFunction(initial_condition, output_times(1));

  var dt_max = 0.05;
  //<=== Initialize  solution <===



  //==== Initialize boundary conditions ====
  var bc = new ZeroFluxDiffusionGridBC(grid = grid);
/*   var bc = new PeriodicGridBC(grid = grid); */



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  solution.clawOutput(frame_number);

  //==== Subsequent times ====
  for output_time in output_times do {
    //==== Advance q to output time ====
    solution.advance_DiffusionBE(bc, diffusivity, output_time, dt_max);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    solution.clawOutput(frame_number);
  }
  //<=== Generate output <===
  

} // end main
