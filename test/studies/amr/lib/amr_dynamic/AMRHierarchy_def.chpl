use LevelSolution_def;
use CFBoundary_def;
use Partitioning;


//|\""""""""""""""""""""""|\
//| >    Flagger class    | >
//|/______________________|/
//----------------------------------------------------------
// Container for the setFlags routine.  This class is meant
// to be derived, and the setFlags routine overridden with
// something user-specified.
//----------------------------------------------------------
class Flagger {
  def setFlags(level_solution: LevelSolution, 
               flags: [level_solution.level.cells] bool) {}
}
// /|""""""""""""""""""""""/|
//< |    Flagger class    < |
// \|______________________\|









//|\"""""""""""""""""""""""""""|\
//| >    AMRHierarchy class    | >
//|/___________________________|/
class AMRHierarchy {

  const x_low, x_high:     dimension*real;
  const n_coarsest_cells:  dimension*int;
  const n_ghost_cells:     dimension*int;

  const max_n_levels: int;
  const ref_ratio:    dimension*int;

  const initialCondition: func(dimension*real, real);
  
  const flagger:  Flagger;
  
  const target_efficiency: real;

  var time: real;

  var level_indices:    domain(1) = [1..1];

  var levels:              [level_indices] Level;
  var fine_boundaries:     [level_indices] CFBoundary;
  var physical_boundaries: [level_indices] PhysicalBoundary;

  var level_solutions:     [level_indices] LevelSolution;
  

  //|\''''''''''''''''''''''''''''''''''''|\
  //| >    Basic methods and iterators    | >
  //|/....................................|/
  def n_levels { return level_indices.numIndices; };

  def coarse_levels {
    for level_idx in 1..n_levels-1 do
      yield level(level_idx);
  }

  def fine_levels {
    for level_idx in 2..n_levels do
      yield level(level_idx);
  }
  
  def coarse_boundaries(i: int) var {
    return fine_boundaries(i-1);
  }
  // /|''''''''''''''''''''''''''''''''''''/|
  //< |    Basic methods and iterators    < |
  // \|....................................\|
    


  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def AMRHierarchy(
    x_low:             dimension*real,
    x_high:            dimension*real,
    n_coarsest_cells:  dimension*int,
    n_ghost_cells:     dimension*int,
    max_n_levels:      int,
    ref_ratio:         dimension*int,
    initialCondition:  func(dimension*real, real),
    flagger:           Flagger,
    target_efficiency: real)
  {

    this.x_low             = x_low;
    this.x_high            = x_high;
    this.n_coarsest_cells  = n_coarsest_cells;
    this.n_ghost_cells     = n_ghost_cells;
    this.max_n_levels      = max_n_levels;
    this.ref_ratio         = ref_ratio;
    this.initialCondition  = initialCondition;
    this.flagger           = flagger;
    this.target_efficiency = target_efficiency;


    //==== Create the top level ====
    levels(1) = new Level(x_low = x_low,  x_high = x_high,
                          n_cells       = n_coarsest_cells,
                          n_ghost_cells = n_ghost_cells);
    writeln("Adding grid to level 1.");                        
    levels(1).addGrid(levels(1).cells);
    levels(1).complete();
    physical_boundaries(1) = new PhysicalBoundary(levels(1));


    //==== Create top solution ====
    level_solutions(1) = new LevelSolution(levels(1));
    level_solutions(1).setToFunction(initialCondition, time);
    
    
    //===> Create refined levels and solutions as needed ===>
    while n_levels < max_n_levels {
      const i_finest: int = n_levels;
      writeln("Refining level ", i_finest, ".");
      const new_level = buildRefinedLevel(i_finest);
      if new_level.grids.numIndices==0 then break;
      
      level_indices = [1..i_finest+1];
      levels(i_finest+1) = new_level;
      
      fine_boundaries(i_finest) = new CFBoundary(levels(i_finest), new_level);
      
      physical_boundaries(i_finest+1) = new PhysicalBoundary(new_level);
      
      level_solutions(i_finest+1) = new LevelSolution(new_level);
      level_solutions(i_finest+1).setToFunction(initialCondition, time);
    }
    //<=== Create refined levels and solutions as needed <===

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|


}
// /|"""""""""""""""""""""""""""/|
//< |    AMRHierarchy class    < |
// \|___________________________\|




//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.buildRefinedLevel    | >
//|/_______________________________________|/
//--------------------------------------------------------
// Regrids the ith level.  This is done by flagging cells
// on level i-1.
//--------------------------------------------------------
def AMRHierarchy.buildRefinedLevel(i_refining: int)
{
  def buffer(flags: [?cells] bool) {
    var buffered_flags: [cells] bool;

    for cell in cells do
      if flags(cell) {
        var ranges: dimension*range(stridable=true);
        for d in dimensions do ranges(d) = cell(d)-4 .. cell(d)+4 by 2;
        var neighborhood: domain(dimension,stridable=true) = ranges;
        for nbr in cells(neighborhood) do
          buffered_flags(nbr) = true;
      }
          
    return buffered_flags;
  }
  
  
  //==== Flag the level being refined====
  var flags: [levels(i_refining).cells] bool;
  flagger.setFlags(level_solutions(i_refining), flags);
  
  //==== Add flags for the level below the new one, if needed ====
  if i_refining+2 <= n_levels {    
    for super_fine_grid in levels(i_refining+2).grids {
      var cells_to_flag = coarsen( coarsen(super_fine_grid.cells, ref_ratio), ref_ratio);
      flags(cells_to_flag) = true;
    }
  }
  
  //==== Add buffer region ====
  var buffered_flags = buffer(flags);
  
  
  //==== Partition ====
  const min_width: dimension*int = 2;
  var partitioned_domains = partitionFlags(buffered_flags, target_efficiency, min_width);
  
  //===> Ensure nesting ===>
  var domains_to_refine = new MultiDomain(dimension,stridable=true);

  for D in partitioned_domains {
    var multi_overflow = new MultiDomain(dimension,stridable=true);
    multi_overflow.add(D);
    
    for grid in levels(i_refining).grids do
      multi_overflow.subtract(grid.cells);
      
    if multi_overflow.domains.numElements == 0 then
      domains_to_refine.add(D);
    else {
      var D_fragments = new MultiDomain(dimension,stridable=true);
      D_fragments.add(D);
      for overflow in multi_overflow do D_fragments.subtract(overflow);
      domains_to_refine.add(D_fragments);
      delete D_fragments;
    }
    
    delete multi_overflow;
  }
  
  delete partitioned_domains;
  //<=== Ensure proper nesting <===
  
  
  //===> Create new level, and return ===>
  var new_level = new Level(x_low   = this.x_low,
                            x_high  = this.x_high,
                            n_cells = levels(i_refining).n_cells * ref_ratio,
                            n_ghost_cells = this.n_ghost_cells);
                            
  for domain_to_refine in domains_to_refine do
    new_level.addGrid( refine(domain_to_refine, ref_ratio) ); //# need to add this method to the level
  
  delete domains_to_refine;
  
  new_level.complete();
  
  return new_level;
  //<=== Create new level, and return <===
  
  
}
// /|"""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.buildRefinedLevel    < |
// \|_______________________________________\|






//|\"""""""""""""""""""""""""""""""|\
//| >    PhysicalBoundary class    | >
//|/_______________________________|/
//---------------------------------------------------------
// Describes the physical boundary of a level, situated in
// the rectangular domain of the full hierarchy.
//---------------------------------------------------------
class PhysicalBoundary {
  const level:        Level;
  const grids:        domain(Grid);
  const multidomains: [grids] MultiDomain(dimension,stridable=true);

  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def PhysicalBoundary(level: Level) {
    for grid in level.grids {

      var boundary_multidomain = new MultiDomain(dimension,stridable=true);
      boundary_multidomain.add(grid.ghost_multidomain);
      boundary_multidomain.subtract(level.cells);

      if boundary_multidomain.domains.numElements > 0 {
        grids.add(grid);
        multidomains(grid) = boundary_multidomain;
      } 
    } // end for grid in level.grids

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|
  
}
// /|"""""""""""""""""""""""""""""""/|
//< |    PhysicalBoundary class    < |
// \|_______________________________\|








//|\""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy output methods    | >
//|/____________________________________|/
//------------------------------------------------------------------------
// Writes full Clawpack output at a given time.  Most of the work is done
// by the "write" method, which handles output of the spatial data.
//------------------------------------------------------------------------
def AMRHierarchy.clawOutput(frame_number: int)
{

  //==== Names of output files ====
  var frame_string:       string = format("%04i", frame_number);
  var time_file_name:     string = "_output/fort.t" + frame_string;
  var solution_file_name: string = "_output/fort.q" + frame_string;


  //==== Time file ====
  var n_grids: int = 0;
  for level in levels do
    n_grids += level.grids.numIndices;

  const time_file = new file(time_file_name, FileAccessMode.write);
  time_file.open();
  writeTimeFile(time, 1, n_grids, 1, time_file);
  time_file.close();


  //==== Solution file ====
  const solution_file = new file(solution_file_name, FileAccessMode.write);
  solution_file.open();
  this.write(solution_file);

}



//----------------------------------------------------------------
// Proceeds down the indexed_levels, calling the LevelArray.write
// method on each corresponding LevelArray.
//----------------------------------------------------------------
def AMRHierarchy.write(outfile: file){

  var base_grid_number = 1;

  for i in level_indices {
    level_solutions(i).current_data.write(i, base_grid_number, outfile);
    base_grid_number += levels(i).grids.numIndices;
  }

}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy output methods    < |
// \|____________________________________\|










def main {
  
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
        for cell in grid.cells {
          var max_differential, differential: real;
          for nbr in neighbors(cell) {
            differential = abs(current_data(grid).value(cell) - current_data(grid).value(nbr));
            max_differential = max(differential, max_differential);
          }
          
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
  
  
  
  const x_low  = (-1.0,-1.0);
  const x_high = (1.0,1.0);
  const n_coarsest_cells = (40,40);
  const n_ghost_cells = (2,2);
  const max_n_levels = 4;
  const ref_ratio = (2,2);
  
  def elevatedSquare (x: 2*real)
  {
    var f: real = 0.0;
    if x(1)<-0.6 && (x(2)>-0.8 && x(2)<-0.2) then f = 1.0;
    return f;
  }
  
  const flagger = new GradientFlagger(tolerance = 0.1);
  const target_efficiency = 0.7;
  
  
  const hierarchy = new AMRHierarchy(x_low,
                                     x_high,
                                     n_coarsest_cells,
                                     n_ghost_cells,
                                     max_n_levels,
                                     ref_ratio,
                                     elevatedSquare,
                                     flagger,
                                     target_efficiency);
  
  hierarchy.clawOutput(0);
  
}