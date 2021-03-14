use AMRHierarchy_AdvectionCTU;
use AMRBC_AdvectionCTU;

use IO;


//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    derived class: GradientFlagger    | >
//|/_______________________________________|/
//-----------------------------------------------------------------
// This Flagger flags a cell if the largest-magnitude differential
// with its neighbors exceeds the threshold 'tolerance'.  It's one
// of the simplest approaches to flagging that's sensible, in a
// somewhat heuristic sense.
//-----------------------------------------------------------------

class GradientFlagger: Flagger {
  
  const tolerance: real = 0.05;

  //|\"""""""""""""""""""""""""|\
  //| >    method: setFlags    | >
  //|/_________________________|/
  //----------------------------------------------------------------------
  // At each cell, this method finds the maximum (magnitude) differential
  // of 'level_solution' with neighboring cells.  The cell is flagged
  // if this value exceeds 'tolerance'.
  //----------------------------------------------------------------------
  
  override proc setFlags (
    level_solution: unmanaged LevelSolution, 
    flags:          [level_solution.level.possible_cells] bool )
  {
    
    //---- Alias and prepare level_solution.current_data ----
    const current_data = level_solution.current_data;
    current_data.extrapolateGhostData();
    current_data.fillOverlaps();
    
    
    //===> Test each cell for flagging ===>
    
    for grid in level_solution.level.grids {

      const ref value = current_data(grid)!.value;
      
      for cell in grid.cells {
        
        //---- Calculate the maximum-magnitude differential ----
        var max_differential, differential: real;
        for nbr in neighbors(cell) {
          differential = abs(value(cell) - value(nbr));
          max_differential = max(differential, max_differential);
        }
        
        //---- Flag if necessary ----
        if max_differential > tolerance {
          flags(cell) = true;
        }
        
      }
    }
    //<=== Test each cell for flagging <===
   
  }
  // /|"""""""""""""""""""""""""/|
  //< |    method: setFlags    < |
  // \|_________________________\|
  
  
  //|\""""""""""""""""""""""""""""|\
  //| >    iterator: neighbors    | >
  //|/____________________________|/
  //-----------------------------------------------------------------
  // This iterates over the neighbors of the input 'cell', where
  // neighbors are defined as sharing an interface of codimension 1.
  // (Hence, only edges count in 2D, faces in 3D, etc.)
  //-----------------------------------------------------------------
  
  iter neighbors ( cell: dimension*int )
  {
    var shift: dimension*int;
    for d in dimensions {
      shift(d) = -2;
      yield cell+shift;
      shift(d) = 2;
      yield cell+shift;
      shift(d) = 0;
    }
  }
  // /|""""""""""""""""""""""""""""/|
  //< |    iterator: neighbors    < |
  // \|____________________________\|
  
}
// /|"""""""""""""""""""""""""""""""""""""""/|
//< |    derived class: GradientFlagger    < |
// \|_______________________________________\|




proc main {
  
  //===> Get names of input files ===>
  
  const pointer_file = open("input_files.txt", iomode.r).reader();
  var time_file_name:      string;
  var hierarchy_file_name: string;
  var velocity_file_name:  string;
  
  pointer_file.read(hierarchy_file_name);
  pointer_file.read(velocity_file_name);
  pointer_file.read(time_file_name);
  pointer_file.close();
  
  writeln(hierarchy_file_name);
  writeln(velocity_file_name);
  
  //<=== Get names of input files <===
  
  
  
  //===> Set up AMR hierarchy ===>

  proc elevatedRectangle (x: dimension*real)
  {
    var f: real = 1.0;
    for d in dimensions do
      if x(d)<-0.8 || x(d) > -0.2 then f = 0.0;
    return f;
  }

  proc elevatedCircle (x: dimension*real) {
    var r: real = 0.0;
    for d in dimensions do
      r += (x(d)+0.5)**2;
    r = sqrt(r);
    if r <= 0.3 then return 1.0;
    else return 0.0;
  }
  
  const flagger = new unmanaged GradientFlagger(tolerance = 0.1);
  const hierarchy = new unmanaged AMRHierarchy(hierarchy_file_name,
                                     flagger,
                                     elevatedCircle);

  //<=== Set up AMR hierarchy <===
  


  //---- Advection velocity ----

  var velocity: dimension*real;
  const velocity_file = open(velocity_file_name, iomode.r).reader();
  velocity_file.read( (...velocity) );
  velocity_file.close();



  //---- Initialize output ----
  var output_times = setOutputTimes(time_file_name);




  //---- Set boundary conditions ----
  var bc = new unmanaged ZeroInflowBC(hierarchy = hierarchy);



  //===> Generate output ===>

  //---- Initial output ----
  var frame_number: int = 0;
  write("Writing initial output...");
  hierarchy.clawOutput(frame_number);
  write("done.\n");
  
  writef("Current time of AMRHierarchy: %8.4Er\n", hierarchy.time);

  
  //---- Subsequent outputs ----
  
  for output_time in output_times(1..) do {

    //---- Advance solution to output time ----
    hierarchy.advance_AdvectionCTU(bc, velocity, output_time);
  
    //---- Write output to file ----
    frame_number += 1;
    write("Writing frame ", frame_number, "...");
    hierarchy.clawOutput(frame_number);
    write("done.\n");

  }

  //<=== Generate output <===
  
  delete bc;
  delete hierarchy;
  delete flagger;
}
