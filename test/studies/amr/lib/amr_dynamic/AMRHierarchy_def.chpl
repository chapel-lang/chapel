use Level_def;
use CFLevelInterface_def;



//|""""""""""""""""""""""""""\
//|===> AMRHierarchy class ===>
//|__________________________/
class AMRHierarchy {

  const x_low, x_high:                   dimension*real;
  const n_coarsest_cells, n_ghost_cells: dimension*int;

  var level_indices:    domain(1) = [1..1];
  var levels:           [level_indices] Level;

  var cf_boundaries:       [level_indices] CFBoundary;
  var physical_boundaries: [level_indices] LevelBoundary;

  

  //|\""""""""""""""""""""""""""""""""""""|\
  //| >    Basic methods and iterators    | >
  //|/____________________________________|/
  def n_levels { return level_indices.numIndices };

  def coarse_levels {
    for level_idx in 1..n_levels-1 do
      yield level(level_idx);
  }

  def fine_levels {
    for level_idx in 2..n_levels do
      yield level(level_idx);
  }
  
  def coarse_boundaries(i: int) var {
    return cf_boundaries(i-1);
  }
  
  def fine_boundaries(i: int) var {
    return cf_boundaries(i);
  }
  // /|""""""""""""""""""""""""""""""""""""/|
  //< |    Basic methods and iterators    < |
  // \|____________________________________\|
    


  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def AMRHierarchy(
    x_low:            dimension*real,
    x_high:           dimension*real,
    n_coarsest_cells: dimension*int,
    n_ghost_cells:    dimension*int)
  {

    this.x_low            = x_low;
    this.x_high           = x_high;
    this.n_coarsest_cells = n_coarsest_cells;
    this.n_ghost_cells    = n_ghost_cells;


    //==== Create the top level ====
    levels(1) = new Level(x_low = x_low,  x_high = x_high,
                          n_cells       = n_coarsest_cells,
                          n_ghost_cells = n_ghost_cells);

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|


}
// /|"""""""""""""""""""""""""""/|
//< |    AMRHierarchy class    < |
// \|___________________________\|





//|\"""""""""""""""""""""""""""""""|\
//| >    PhysicalBoundary class    | >
//|/_______________________________|/
//---------------------------------------------------------
// Describes the physical boundary of a level, situated in
// the rectangular domain of the full hierarchy.
//---------------------------------------------------------
class PhysicalBoundary {
  const level:               Level;
  var   grids:               domain(Grid);
  var   domain_sets: [grids] DomainSet(dimension,true);

  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def PhysicalBoundary(level: Level) {
    for grid in level.grids {

      var boundary_domain_set = new DomainSet(dimension,true);
      for D in grid.ghost_domain_set do
        boundary_domain_set.add(D);

      boundary_domain_set -= level.cells;

      if boundary_domain_set.indices.numIndices > 0 {
        grids.add(grid);
        domain_sets(grid) = boundary_domain_set;
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






//|\""""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.addRefinedLevel method    | >
//|/____________________________________________|/
//---------------------------------------------------------------------
// Provided a refinement ratio, this creates a new level which refines
// the current bottom level.
//---------------------------------------------------------------------
def AMRHierarchy.addRefinedLevel(ref_ratio: dimension*int) {
  
  //==== Build refined level (new_bottom_level) ====
  //----------------------------------------------------------------------
  // The number of cells for new_bottom_level is found by applying the 
  // refiement ratio to old_bottom_level's n_cells.
  //----------------------------------------------------------------------
  var n_refined_cells  = levels(n_levels).n_cells * ref_ratio;

  levels = [1..n_levels+1];
  var levels(n_levels) = new Level(x_low = x_low, x_high = x_high,
                                   n_cells       = n_refined_cells,
                                   n_ghost_cells = n_ghost_cells);


  //==== Build new CFBoundary ====
  //-------------------------------------------------------------
  // Note that the CFBoundary has information on grid structure,
  // which will need to be filled in later.
  //-------------------------------------------------------------
  cf_boundaries(n_levels-1) = new CFBoundary(coarse_level = old_bottom_level,
                                             fine_level   = new_bottom_level);
                                             
}
// /|""""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.addRefinedLevel method    < |
// \|____________________________________________\|




//|\"""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.createLevel    | >
//|/_________________________________|/
def AMRHierarchy.createLevel(
  i:               int,
  ref_ratio_above: dimension*int)
{
  
  var n_refined_cells  = levels(i-1).n_cells * ref_ratio;
  return = new Level(x_low = x_low, x_high = x_high,
                     n_cells       = n_refined_cells,
                     n_ghost_cells = n_ghost_cells);

}
// /|"""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.createLevel    < |
// \|_________________________________\|




//|\"""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.addGridToLevel method    | >
//|/___________________________________________|/
//-----------------------------------------------------------------------
// Adds a grid with corners at x_low and x_high to the input level,
// snapped to the nearest vertices of the coarser level, if applicable.
// This ensures proper blocking of the grid.  In a full AMR system, that
// should be handled by the regridding algorithm, but this simplifies
// the manual development of some test hierarchies.
//-----------------------------------------------------------------------
def AMRHierarchy.addGridToLevel(
  level:  Level,
  x_low:  dimension*real,
  x_high: dimension*real)
{

  //==== Find index bounds for new grid ====
  var i_low, i_high: dimension*int;
  var level_idx = levelIndex(level);

  if level_idx == 1 {
    i_low  = level.snapToVertex(x_low);
    i_high = level.snapToVertex(x_high);
  }
  else {
    var coarser_level = levels(level_idx-1);
    var ref_ratio = refinementRatio(coarser_level, level);
    i_low  = coarser_level.snapToVertex(x_low) * ref_ratio;
    i_high = coarser_level.snapToVertex(x_high) * ref_ratio;
  }


  //==== Create new grid ====
  level.addGrid(i_low, i_high);

}
// /|"""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.addGridToLevel method    < |
// \|___________________________________________\|







//|\""""""""""""""""""""""""""""""""""""""""""|\
//| >    AMRHierarchy.completeLevel method    | >
//|/__________________________________________|/
//---------------------------------------------------------------------
// After all grids have been added to a level, it should be completed. 
// At this point, it is safe to compute its boundary ghosts, and to
// complete its coarse interface.
//---------------------------------------------------------------------
def AMRHierarchy.completeLevel(level: Level) {

  writeln("Completing level ", level_numbers(level));

  level.complete();

  level_idx = levelIndex(level);

  physical_boundaries(level_idx) = new PhysicalBoundary(level);

  if level_idx != 1 then
    coarse_interfaces(level_idx).complete();
}
// /|""""""""""""""""""""""""""""""""""""""""""/|
//< |    AMRHierarchy.completeLevel method    < |
// \|__________________________________________\|












//|"""""""""""""""""""""""""""""\
//|===> readHierarchy routine ===>
//|_____________________________/
def readHierarchy(file_name: string){
 
  //==== Open input file ====
  var input_file = new file(file_name, FileAccessMode.read);
  input_file.open();


  //==== Safety check the dimension ====
  var dim_input: int;
  input_file.readln(dim_input);
  assert(dim_input == dimension,
         "error: dimension of input file inconsistent with compiled dimension.");
  input_file.readln(); // skip line


  //==== Problem domain / Top level ====
  var x_low, x_high: dimension*real;
  var n_cells, n_ghost_cells: dimension*int;
  input_file.readln( (...x_low) );
  input_file.readln( (...x_high) );
  input_file.readln( (...n_cells) );
  input_file.readln( (...n_ghost_cells) );
  input_file.readln(); // skip line

  var hierarchy = new AMRHierarchy(x_low = x_low, x_high = x_high,
                                   n_coarsest_cells = n_cells,
                                   n_ghost_cells = n_ghost_cells);

  hierarchy.addGridToLevel(hierarchy.top_level, x_low, x_high);
  hierarchy.completeLevel(hierarchy.top_level);
  

  //===> Refined levels ===>
  var n_levels: int;
  input_file.readln(n_levels);
  input_file.readln(); // skip line

  var ref_ratio: dimension*int;
  var n_grids:   int;

  for i_level in [2..n_levels] {

    input_file.readln( (...ref_ratio) );
    input_file.readln( n_grids );
    input_file.readln(); // skip line

    hierarchy.addRefinedLevel(ref_ratio);

    for i_grid in [1..n_grids] {
      input_file.readln( (...x_low) );
      input_file.readln( (...x_high) );
      input_file.readln(); // skip line

      hierarchy.addGridToLevel(hierarchy.bottom_level, x_low, x_high);
    }

    hierarchy.completeLevel(hierarchy.bottom_level);

  }
  //<=== Refined levels <===


  //==== Close input file and return ====
  input_file.close();
  return hierarchy;

}
// /"""""""""""""""""""""""""""""/
//<=== readHierarchy routine <==<
// \_____________________________\








def main {

  var hierarchy = readHierarchy("space.txt");
  var level = hierarchy.top_level;

  while level {
      
    writeln("Level number ", hierarchy.level_numbers(level), ":");
    writeln(level);
    writeln("");

    if level != hierarchy.bottom_level then
      level = hierarchy.fine_interfaces(level).fine_level;
    else
      level = nil;
  }

}