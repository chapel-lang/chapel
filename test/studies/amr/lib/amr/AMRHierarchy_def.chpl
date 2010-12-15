use LevelSolution_def;
use CoarseOverlapSolution_def;
use Partitioning;




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
  
  const flagger:  Flagger;  // Flagger class is defined below
  
  const target_efficiency: real;


  var time: real;

  var level_indices: domain(1) = [1..1];

  var levels:                   [level_indices] Level;
  var level_solutions:          [level_indices] LevelSolution;


  //==== Boundary structures ====
  var coarse_boundaries:        [level_indices] CFBoundary;
  var coarse_overlap_solutions: [level_indices] CoarseOverlapSolution;
  var physical_boundaries:      [level_indices] PhysicalBoundary;


  const steps_before_regrid = 2;
  var regrid_counters: [level_indices] int;
  

  //|\''''''''''''''''''''''''''''''''''''|\
  //| >    Basic methods and iterators    | >
  //|/....................................|/
  def n_levels { return level_indices.high; };
  
  def fine_boundaries(i: int) var {
    return coarse_boundaries(i+1);
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
    target_efficiency: real,
    flagger:           Flagger,
    initialCondition:  func(dimension*real, real))
  {

    this.x_low             = x_low;
    this.x_high            = x_high;
    this.n_coarsest_cells  = n_coarsest_cells;
    this.n_ghost_cells     = n_ghost_cells;
    this.max_n_levels      = max_n_levels;
    this.ref_ratio         = ref_ratio;
    this.target_efficiency = target_efficiency;
    this.flagger           = flagger;
    this.initialCondition  = initialCondition;


    //==== Create the top level ====
    levels(1) = new Level(x_low = x_low,  x_high = x_high,
                          n_cells       = n_coarsest_cells,
                          n_ghost_cells = n_ghost_cells);
    writeln("Adding grid to level 1.");                        
    levels(1).addGrid(levels(1).possible_cells);
    levels(1).complete();
    physical_boundaries(1) = new PhysicalBoundary(levels(1));


    //==== Create top solution ====
    level_solutions(1) = new LevelSolution(levels(1));
    level_solutions(1).setToFunction(initialCondition, time);
    
    
    //===> Create refined levels and solutions as needed ===>
    var i_finest = 1;
    while i_finest < max_n_levels {
      
      writeln("Refining level ", i_finest, ".");
      const new_level = buildRefinedLevel(i_finest);
            
      if new_level.grids.numIndices>0 {
        i_finest += 1;
        level_indices = [1..i_finest];
        
        //==== Create new level ====
        levels(i_finest) = new_level;

        //==== Create new solution ====
        level_solutions(i_finest) = new LevelSolution(new_level);
        level_solutions(i_finest).setToFunction(initialCondition, time);
        
      
        //==== Create new boundary structures ====
        coarse_boundaries(i_finest)        = new CFBoundary(levels(i_finest-1), new_level);
        coarse_overlap_solutions(i_finest) = new CoarseOverlapSolution(coarse_boundaries(i_finest));
        physical_boundaries(i_finest)      = new PhysicalBoundary(new_level);
      
        //==== Initialize regrid counter ====
        regrid_counters(i_finest) = steps_before_regrid;
      }
      else {
        writeln("Refinement unnecessary.");
        break;
      }
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


//|\""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.regrid    | >
//|/____________________________|/
def AMRHierarchy.regrid(
  i_base: int)
{

  writeln("regrid called from base level ", i_base, ".");

  const i_finest_old = n_levels;
  var   i_finest     = i_base;

  //===> Create new level of refinement, if possible and necessary ===>  
  if i_finest_old < max_n_levels {
    writeln("Refining level ", i_finest_old, ".");

    //==== Create new level ====
    const new_level = buildRefinedLevel(i_finest_old);
  
    //==== If level is nonempty, then add it to the hierarchy ====
    if new_level.grids.numIndices>0 {
      //==== Update indices ====
      i_finest      = i_finest_old+1;
      level_indices = [1..i_finest];
      
      //==== Add new level ====
      levels(i_finest) = new_level;

      //==== Create and fill new solution ====
      level_solutions(i_finest) = new LevelSolution(new_level);
      level_solutions(i_finest+1).initialFill( level_solutions(i_finest) );

      //==== Create new boundary structures ====
      coarse_boundaries(i_finest)        = new CFBoundary(levels(i_finest-1), new_level);
      coarse_overlap_solutions(i_finest) = new CoarseOverlapSolution( coarse_boundaries(i_finest) );
      physical_boundaries(i_finest)      = new PhysicalBoundary(new_level);
      
      //==== Set regrid counter ====
      regrid_counters(i_finest) = steps_before_regrid;    
      
      writeln("Level ", i_finest, " created!");  
    }
    else
      writeln("Refinement unnecessary.");
  }
  //<=== Create new level of refinement, if possible and necessary <===
  
  
  //===> Regrid existing levels ===>
  var i_regridding = i_finest_old;
  while i_regridding > i_base {
    writeln("Regridding level ", i_regridding, ".");
    
    //==== Remove old boundary structures ====
    coarse_boundaries(i_regridding).clear();
    delete coarse_boundaries(i_regridding);

    coarse_overlap_solutions(i_regridding).clear();
    delete coarse_overlap_solutions(i_regridding);
        
    physical_boundaries(i_regridding).clear();
    delete physical_boundaries(i_regridding);

    
    //==== Create new level ====
    const regridded_level = buildRefinedLevel(i_regridding-1);
    writeln("Built regridded level.");

    if regridded_level.grids.numIndices>0 {
      //==== Update the finest level index ====
      i_finest = max(i_finest, i_regridding);
            
      //==== Replace the solution ====
      var regridded_level_solution = new LevelSolution(regridded_level);
      regridded_level_solution.initialFill( level_solutions(i_regridding), 
                                            level_solutions(i_regridding-1) );
                                                                                
      level_solutions(i_regridding).clear();
      delete level_solutions(i_regridding);
      level_solutions(i_regridding) = regridded_level_solution;
            
      //==== Replace the level ====
      levels(i_regridding).clear();
      delete levels(i_regridding);
      levels(i_regridding) = regridded_level;
      
      //==== Set regrid counter ====
      regrid_counters(i_regridding) = steps_before_regrid;
      
      writeln("Level ", i_regridding, " successfully regridded.");
    }
    else {
      //==== Clear old solution ====
      level_solutions(i_regridding).clear();
      delete level_solutions(i_regridding);
      
      //==== Clear the old level ====
      levels(i_regridding).clear();
      delete levels(i_regridding);
      
      //==== Shrink the index space ====
      //--------------------------------------------------------------
      // Need to do this now rather than one shrinking operation at
      // the end, as buildRefinedLevel depends on each element of the
      // index space corresponding to an actual level.
      //--------------------------------------------------------------
      level_indices = [1..i_regridding-1];
      
      writeln("Level ", i_regridding, "no longer needed -- discarded.");
    }
    
    //==== Shift the regridding index up ====
    i_regridding -= 1;
  }
  //<=== Regrid existing levels <===
  
  
  //==== Create new boundary structures ====
  for i in i_base+1..i_finest {
    coarse_boundaries(i)        = new CFBoundary(levels(i-1), levels(i));
    coarse_overlap_solutions(i) = new CoarseOverlapSolution( coarse_boundaries(i) );
    physical_boundaries(i)      = new PhysicalBoundary( levels(i) );
  }
  
}
// /|""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.regrid    < |
// \|____________________________\|




//|\"""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.buildRefinedLevel    | >
//|/_______________________________________|/

//--------------------------------------------------------
// Builds a refined level below level i_refining.
//--------------------------------------------------------

def AMRHierarchy.buildRefinedLevel(i_refining: int)
{
  def buffer(flags: [?cells] bool) {
    var buffered_flags: [cells] bool;

    for cell in cells do
      if flags(cell) {
        var ranges: dimension*range(stridable=true);
        for d in dimensions do ranges(d) = cell(d)-2 .. cell(d)+2 by 2;
        var neighborhood: domain(dimension,stridable=true) = ranges;
        for nbr in cells(neighborhood) do
          buffered_flags(nbr) = true;
      }
          
    return buffered_flags;
  }
  
  
  
  //==== Flag the level being refined====
  var flags: [levels(i_refining).possible_cells] bool;
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
  
  
  //===> Ensure proper nesting ===>
  //------------------------------------------------------------------------
  // Every domain to refine must be at least one cell away from the level's
  // boundary with a coarser level (physical boundary is OK).  So for each
  // partitioned domain, we locate the "adjacent coarse region," in terms
  // of cells sized by the refining level that spill into the next-coarser
  // level.
  //
  // This is done by growing the partitioned domain by 1 cell, and then
  // subtracting the (interior) cells of every grid on the level.  The
  // level's physical boundary is also removed.  Then
  // this adjacent coarse region is expanded by 1 cell and removed from
  // the original partitioned domain.
  //
  // (You'll probably want to draw a picture.  Verbal explanation doesn't
  // do it justice.) 
  //------------------------------------------------------------------------

  var domains_to_refine = new MultiDomain(dimension,stridable=true);

  for D in partitioned_domains {
    var adjacent_coarse_region = new MultiDomain(dimension,stridable=true);
    
    //==== Initialize to D expanded by one cell ====
    adjacent_coarse_region.add( D.expand(2) );

    //==== Physical boundary is OK; this trims it off ====
    adjacent_coarse_region.intersect( levels(i_refining).possible_cells );

    //==== Remove all grid cells from the level ====
    for grid in levels(i_refining).grids do
      adjacent_coarse_region.subtract(grid.cells);
      
    var properly_nested_domains = new MultiDomain(dimension,stridable=true);
    properly_nested_domains.add(D);
    
    for adjacent_coarse_domain in adjacent_coarse_region do
      properly_nested_domains.subtract( adjacent_coarse_domain.expand(2) );
    
    domains_to_refine.add( properly_nested_domains );
    
    delete adjacent_coarse_region;
    delete properly_nested_domains;
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


  //|\''''''''''''''''|\
  //| >    clear()    | >
  //|/................|/
  def clear() {
    for grid in grids do delete multidomains(grid);
  }
  // /|''''''''''''''''/|
  //< |    clear()    < |
  // \|................\|


  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def PhysicalBoundary(level: Level) {
    for grid in level.grids {

      var boundary_multidomain = new MultiDomain(dimension,stridable=true);
      boundary_multidomain.add(grid.ghost_multidomain);
      boundary_multidomain.subtract(level.possible_cells);

      if boundary_multidomain.domains.numElements > 0 {
        grids.add(grid);
        multidomains(grid) = boundary_multidomain;
      } 
    } // end for grid in level.grids

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|
  
  
  //|\'''''''''''''''''''''''|\
  //| >    these iterator    | >
  //|/.......................|/
  def these() {
    for grid in grids do
      yield (grid, multidomains(grid));
  }
  // /|'''''''''''''''''''''''/|
  //< |    these iterator    < |
  // \|.......................\|
  
}
// /|"""""""""""""""""""""""""""""""/|
//< |    PhysicalBoundary class    < |
// \|_______________________________\|




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
               flags: [level_solution.level.possible_cells] bool) {}
}
// /|""""""""""""""""""""""/|
//< |    Flagger class    < |
// \|______________________\|



//|\"""""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy constructor, file-based    | >
//|/_____________________________________________|/

//-----------------------------------------------------------------
// Alternate constructor in which all numerical parameters for the
// hierarchy are provided using an input file.  This allows those
// parameters to be changed without recompiling the code.
//-----------------------------------------------------------------

def AMRHierarchy.AMRHierarchy(
  file_name:  string,
  flagger:    Flagger,
  inputIC:    func(dimension*real,real))
{

  const parameter_file = new file(file_name, FileAccessMode.read);
  parameter_file.open();

  //==== Safety check the dimension ====
  var dim_input: int;
  parameter_file.readln(dim_input);
  assert(dim_input == dimension,
         "error: dimension of input file inconsistent with compiled dimension.");
  parameter_file.readln(); // skip line


  //==== Read parameters from file ====
  var x_low, x_high:     dimension*real;
  var n_coarsest_cells:  dimension*int;
  var n_ghost_cells:     dimension*int;
  var max_n_levels:      int;
  var ref_ratio:         dimension*int;
  var target_efficiency: real;

  parameter_file.readln( (...x_low) );
  parameter_file.readln( (...x_high) );
  parameter_file.readln( (...n_coarsest_cells) );
  parameter_file.readln( (...n_ghost_cells) );
  parameter_file.readln( (...ref_ratio) );
  parameter_file.readln( max_n_levels );
  parameter_file.readln( target_efficiency );
  parameter_file.close();

  

  //==== Create and return hierarchy ====
  return new AMRHierarchy(x_low,
                          x_high,
			  n_coarsest_cells,
			  n_ghost_cells,
			  max_n_levels,
			  ref_ratio,
			  target_efficiency,
			  flagger,
			  inputIC);

}
// /|"""""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy constructor, file-based    < |
// \|_____________________________________________\|





//|\""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.initialFill    | >
//|/__________________________________|/
def LevelSolution.initialFill(
  old_solution:    LevelSolution,
  coarse_solution: LevelSolution)
{
  assert( abs(old_solution.current_time - coarse_solution.current_time) < 1.0e-8);
  
  current_data.initialFill(old_solution.current_data, coarse_solution.current_data);
  current_time = old_solution.current_time;
  old_time     = current_time;
}

def LevelSolution.initialFill(
  coarse_solution: LevelSolution)
{
  current_data.initialFill(coarse_solution.current_data);
  current_time = coarse_solution.current_time;
  old_time     = current_time;
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.initialFill    < |
// \|__________________________________\|


//|\""""""""""""""""""""""""""""""""""|\
//| >    LevelVariable.initialFill    | >
//|/__________________________________|/
def LevelVariable.initialFill(
  q_old:     LevelVariable,
  q_coarse:  LevelVariable)
{
  //==== Safety check ====
  if q_old != nil then
    assert(this.level.n_cells == q_old.level.n_cells);
  
  //==== Refinement ratio ====
  const ref_ratio = refinementRatio(q_coarse.level, this.level);
  
  //===> Fill grid-by-grid ===>
  for grid in level.grids {
    //==== Initialize unfilled cell blocks ====
    var unfilled_cell_blocks = new MultiDomain(dimension, true);
    unfilled_cell_blocks.add(grid.cells);

    //===> Copy from q_old where possible ===>
    if q_old != nil then
      for old_grid in q_old.level.grids {
      
        var overlap = grid.cells( old_grid.cells );

        if overlap.numIndices > 0 {
          this(grid,overlap) = q_old(old_grid, overlap);
          unfilled_cell_blocks.subtract(overlap);
        }
      }
    //<=== Copy from q_old where possible <===
  
    //===> Interpolate from q_coarse everywhere else ===>
    for coarse_grid in q_coarse.level.grids {
      
      //==== Calculate full overlap on the fine grid ====
      var overlap = grid.cells( refine(coarse_grid.cells, ref_ratio) );
      
      //==== If there is an overlap, remove fragments that coincide with unfilled_cell_blocks ====
      if overlap.numIndices > 0 {        
        for block in unfilled_cell_blocks {
          var unfilled_overlap = overlap(block);
          if unfilled_overlap.numIndices>0 then
            this(grid,unfilled_overlap) 
                = q_coarse(coarse_grid).refine_Linear(unfilled_overlap, ref_ratio);          
        }
        
        unfilled_cell_blocks.subtract(overlap);
      }
      
    }
    //<=== Interpolate from q_coarse everywhere else <===

  }
  //<=== Fill grid-by-grd <===
}


def LevelVariable.initialFill(
  q_coarse: LevelVariable)
{
  const q_old: LevelVariable;
  initialFill(q_old, q_coarse);
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    LevelVariable.initialFill    < |
// \|__________________________________\|







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
  this.writeData(solution_file);

}



//----------------------------------------------------------------
// Proceeds down the indexed_levels, calling the LevelVariable.write
// method on each corresponding LevelVariable.
//----------------------------------------------------------------
def AMRHierarchy.writeData(outfile: file){

  var base_grid_number = 1;

  for i in level_indices {
    level_solutions(i).current_data.writeData(i, base_grid_number, outfile);
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
      flags:          [level_solution.level.possible_cells] bool)
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
				     target_efficiency,
				     flagger,
				     elevatedSquare);
  
  hierarchy.clawOutput(0);
  
}