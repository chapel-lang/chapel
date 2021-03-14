//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
// The spatial dimension is a parameter defined in grid_base_defs.chpl.
//
//<=== Description <===


use GridSolution_AdvectionCTU;
use GridBC_AdvectionCTU;


proc main {

  //==== Output times ====
  write("Reading output times...");
  var output_times = setOutputTimes("time.txt");
  write("done.\n");
  

  //==== Grid ====
  writeln("Setting up grid.");
  var grid = readGrid("grid.txt");
  writeln("Done setting up grid.");


  //===> Initialize solution ===>
  proc initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
    	f *= exp(-30*x(d)**2);
    return f;
  }

  var sol = new unmanaged GridSolution(grid = grid);

  sol.setToFunction(initial_condition, output_times(0));
  //<=== Initialize solution <===



  //==== Advection velocity ====
  //  var velocity = readVelocity("parameters.txt");
  var velocity: dimension*real;
  [d in dimensions with (ref velocity)] // could also be 'for param d'
    velocity(d) = 0.5 + 0.5**(d+1);



  //==== Initialize boundary conditions ====
/*   var bc = new ZeroInflowAdvectionGridBC(grid = grid); */
  var bc = new unmanaged PeriodicGridBC(grid = grid);



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  sol.clawOutput(frame_number);

  //==== Subsequent times ====
  for output_time in output_times(1..) do {
    //==== Advance solution to output time ====
    sol.advance_AdvectionCTU(bc, velocity, output_time);

    //==== Write output to file ====
    frame_number += 1;
    writeln("Writing frame ", frame_number, ".");
    sol.clawOutput(frame_number);
  }
  //<=== Generate output <===

  delete sol;
  delete bc;
  delete grid;
} // end main




