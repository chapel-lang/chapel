
use CFBoundary_def;



//|\""""""""""""""""""""""""""""""""|\
//| >    StaticMRHierarchy class    | >
//|/________________________________|/
class StaticMRHierarchy {

  const x_low, x_high:                   dimension*real;
  const n_coarsest_cells, n_ghost_cells: dimension*int;

  var level_indices: domain(1) = [1..1];
  var levels:        [level_indices] Level;

  var fine_boundaries:     [level_indices] CFBoundary;
  var physical_boundaries: [level_indices] PhysicalBoundary;


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
  //| >    Constructor    | >
  //|/....................|/
  def StaticMRHierarchy(
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
    write("Creating top level...");
    levels(1) = new Level(x_low = x_low,  x_high = x_high,
                          n_cells       = n_coarsest_cells,
                          n_ghost_cells = n_ghost_cells);
    levels(1).addGrid(levels(1).cells);
    levels(1).complete();
    
    physical_boundaries(1) = new PhysicalBoundary(levels(1));
    write("done.\n");

  }
  // /|''''''''''''''''''''/|
  //< |    Constructor    < |
  // \|....................\|


}
// /|""""""""""""""""""""""""""""""""/|
//< |    StaticMRHierarchy class    < |
// \|________________________________\|





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






//|\"""""""""""""""""""""""""""""""""""|\
//| >    StaticMRHierarchy.addLevel    | >
//|/___________________________________|/
//---------------------------------------------------------------------
// Provided a refinement ratio, this creates a new level which refines
// the current bottom level.
//---------------------------------------------------------------------
def StaticMRHierarchy.addLevel(ref_ratio: dimension*int) {
  
  var i_finest = level_indices.high;
  var n_refined_cells  = levels(i_finest).n_cells * ref_ratio;


  level_indices = [1..i_finest+1];
  levels(i_finest+1) = new Level(x_low = x_low, x_high = x_high,
                                 n_cells       = n_refined_cells,
                                 n_ghost_cells = n_ghost_cells);

}
// /|""""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRHierarchy.addLevel method    < |
// \|__________________________________________\|






//|\"""""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRHierarchy.addGrid method    | >
//|/_________________________________________|/
//-----------------------------------------------------------------------
// Adds a grid with corners at x_low and x_high to the input level,
// snapped to the nearest vertices of the coarser level, if applicable.
// This ensures proper blocking of the grid.  In a full AMR system, that
// should be handled by the regridding algorithm, but this simplifies
// the manual development of some test hierarchies.
//-----------------------------------------------------------------------
def StaticMRHierarchy.addGrid(
  x_low:  dimension*real,
  x_high: dimension*real)
{

  writeln("Adding grid for x_low=", x_low, ", x_high=", x_high, ".");

  const bottom_idx = level_indices.high;

  //==== Safety check ====
  assert(levels(bottom_idx).is_complete == false);

  //==== Calculate grid index bounds with proper blocking ====
  const ref_ratio = refinementRatio(levels(bottom_idx-1), levels(bottom_idx));
  const i_low  = levels(bottom_idx-1).snapToVertex(x_low) * ref_ratio;
  const i_high = levels(bottom_idx-1).snapToVertex(x_high) * ref_ratio;
  
  
  //==== Add new grid ====
  levels(bottom_idx).addGrid(i_low, i_high);

}
// /|"""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRHierarchy.addGrid method    < |
// \|_________________________________________\|







//|\"""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    StaticMRHierarchy.completeLevel method    | >
//|/_______________________________________________|/
//---------------------------------------------------------------------
// After all grids have been added to a level, it should be completed. 
// At this point, it is safe to compute its boundary ghosts, and to
// complete its coarse interface.
//---------------------------------------------------------------------
def StaticMRHierarchy.completeLevel() {

  const i_bottom = level_indices.high;
  
  //==== Safety check ====
  assert(levels(i_bottom).is_complete == false);


  //==== Complete level ====
  write("Completing level ", i_bottom, "...");
  levels(i_bottom).complete();
  write("done.\n");
  
  //==== Set overlap and boundary information ====
  writeln("Adding CFBoundary.");
  fine_boundaries(i_bottom-1) = new CFBoundary(levels(i_bottom-1), levels(i_bottom));
  writeln("Adding PhysicalBoundary.");
  physical_boundaries(i_bottom) = new PhysicalBoundary(levels(i_bottom));

}
// /|"""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    StaticMRHierarchy.completeLevel method    < |
// \|_______________________________________________\|










//|\""""""""""""""""""""""""""""""""""""""|\
//| >    readStaticMRHierarchy routine    | >
//|/______________________________________|/
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


  //==== Hierarchy parameters ====
  var x_low, x_high: dimension*real;
  var n_cells, n_ghost_cells: dimension*int;
  input_file.readln( (...x_low) );
  input_file.readln( (...x_high) );
  input_file.readln( (...n_cells) );
  input_file.readln( (...n_ghost_cells) );
  input_file.readln(); // skip line


  //==== Build hierarchy; top level generated automatically ====
  var hierarchy = new StaticMRHierarchy(x_low = x_low, x_high = x_high,
                                        n_coarsest_cells = n_cells,
                                        n_ghost_cells = n_ghost_cells);
  

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

    hierarchy.addLevel(ref_ratio);

    for i_grid in [1..n_grids] {
      input_file.readln( (...x_low) );
      input_file.readln( (...x_high) );
      input_file.readln(); // skip line

      hierarchy.addGrid(x_low, x_high);
    }

    hierarchy.completeLevel();

  }
  //<=== Refined levels <===


  //==== Close input file and return ====
  input_file.close();
  return hierarchy;

}
// /|""""""""""""""""""""""""""""""""""""""/|
//< |    readStaticMRHierarchy routine    < |
// \|______________________________________\|








def main {

  var hierarchy = readHierarchy("input_hierarchy.txt");

  for i in hierarchy.level_indices {      
    writeln("Level number ", i, ":");
    writeln(hierarchy.levels(i));
    writeln("");
  }

}