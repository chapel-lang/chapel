//===> Description ===>
//
// Driver for an advection example, integrated with corner transport
// upwind (CTU).
//
//<=== Description <===


use LevelSolution_AdvectionCTU;
use LevelBC_AdvectionCTU;



proc main {


  //==== Initialize output ====
  var output_times = setOutputTimes("set_problem/time.txt");


  //===> Initialize space ===>  
  var level = readLevel("set_problem/space.txt");



  //===> Initialize solution ===>
  write("Initializing solution...");
  proc initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30 * (x(d) + 0.5)**2);
    return f;
  }

  var solution = new LevelSolution(level = level);
  solution.setToFunction(initial_condition, output_times(0));
  write("done.\n");
  //<=== Initialize  solution <===



  //===> Advection velocity ===>
  var velocity: dimension*real;
  var phys_file = open("set_problem/physics.txt", iomode.r).reader();
  var dim_in: int;
  phys_file.readln(dim_in);
  assert(dim_in == dimension, 
         "error: dimension of physics.txt must equal " + "%i".format(dimension));
  phys_file.readln((...velocity));
  phys_file.close();
  //<=== Advection velocity <===



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroInflowBC(level = level);
  write("done.\n");



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  write("Writing initial output...");
  solution.clawOutput(frame_number);
  write("done.\n");
  
  //==== Subsequent times ====
  for output_time in output_times(1..) do {
    //==== Advance solution to output time ====
    solution.advance_AdvectionCTU(bc, velocity, output_time);
  
    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    solution.clawOutput(frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  
  

} // end main
