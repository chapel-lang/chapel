use StaticMRSolution_AdvectionCTU;
use StaticMRBC_AdvectionCTU;



proc main {
  

  //==== Initialize output ====
  var output_times = setOutputTimes("set_problem/time.txt");


  //===> Initialize space ===>  
  var hierarchy = readHierarchy("set_problem/hierarchy.txt");



  //===> Initialize solution ===>
  write("Initializing solution...");
  proc initial_condition ( x: dimension*real ) {
    var f: real = 1.0;
    for d in dimensions do
      f *= exp(-30 * (x(d) + 0.7)**2);
    return f;
  }

  var solution = new StaticMRSolution(hierarchy = hierarchy);
  solution.setToFunction(initial_condition, output_times(0));
  write("done.\n");
  //<=== Initialize  solution <===



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



  //==== Set boundary conditions ====
  write("Setting boundary conditions...");
  var bc = new ZeroInflowBC(hierarchy = hierarchy);
  write("done.\n");



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  write("Writing initial output...");
  solution.clawOutput(frame_number);
  write("done.\n");
  
  writeln("Current time of AMRSolution: " + format("%8.4E",solution.time));
  
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
  
  
}


