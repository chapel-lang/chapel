use AMRHierarchy_AdvectionCTU;
use AMRBC_AdvectionCTU;


//===> Flagger definition ===>
class GradientFlagger: Flagger {
  
  const tolerance: real = 0.05;
  
  def setFlags(
    level_solution: LevelSolution, 
    flags:          [level_solution.level.cells] bool)
  {
    const current_data = level_solution.current_data;
    current_data.extrapolateGhostData();
    
    for grid in level_solution.level.grids {
      const value => current_data(grid).value;
      
      for cell in grid.cells {
        var max_differential, differential: real;
        for nbr in neighbors(cell) {
          differential = abs(value(cell) - value(nbr));
          max_differential = max(differential, max_differential);
        }

        // writeln("cell: ", cell, ";  value = ", current_data(grid).value(cell));        
        // writeln("cell: ", cell, ";  max_differential = ", max_differential);
        
        if max_differential > tolerance {
          flags(cell) = true;
          // writeln("Adding flag");
        }
      }
    }
    
  }
  
  def neighbors(cell: dimension*int) {
    var shift: dimension*int;
    for d in dimensions {
      shift(d) = -2;
      yield cell+shift;
      shift(d) = 2;
      yield cell+shift;
      shift(d) = 0;
    }
  }
}
//<=== Flagger definition <===




def main {
  
  //===> Set up AMR hierarchy ===> */
  
  def elevatedRectangle (x: dimension*real)
  {
    var f: real = 1.0;
    for d in dimensions do
      if x(d)<-0.8 || x(d) > -0.2 then f = 0.0;
    return f;
  }

  def elevatedCircle (x: dimension*real) {
    var r: real = 0.0;
    for d in dimensions do
      r += (x(d)+0.5)**2;
    r = sqrt(r);
    if r <= 0.3 then return 1.0;
    else return 0.0;
  }

  
  const flagger = new GradientFlagger(tolerance = 0.1);
  const hierarchy = new AMRHierarchy('set_problem/hierarchy_parameters.txt',
				     flagger,
				     elevatedCircle);
  //<=== Set up AMR hierarchy <===
  

  //==== Initialize output ====
  var output_times = setOutputTimes("set_problem/time.txt");


  //==== Advection velocity ====
  var velocity: dimension*real = (0.75, 0.625, 0.5);


  //==== Set boundary conditions ====
  var bc = new ZeroInflowBC(hierarchy = hierarchy);



  //===> Generate output ===>
  //==== Initial time ====
  var frame_number: int = 0;
  write("Writing initial output...");
  hierarchy.clawOutput(frame_number);
  write("done.\n");
  
  writeln("Current time of AMRHierarchy: " + format("%8.4E",hierarchy.time));
  
  //==== Subsequent times ====
  for output_time in output_times(1..) do {
    //==== Advance solution to output time ====
    hierarchy.advance_AdvectionCTU(bc, velocity, output_time);
  
    //==== Write output to file ====
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    hierarchy.clawOutput(frame_number);
    write("done.\n");
  }
  //<=== Generate output <===
  
  
}


