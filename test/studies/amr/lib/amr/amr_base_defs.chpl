use level_base_defs;
use level_interface_defs;



//|""""""""""""""""""""""""""\
//|===> AMRHierarchy class ===>
//|__________________________/
class AMRHierarchy {

  const x_low, x_high:                  dimension*real;
  const n_coarsest_cells, n_ghost_cells: dimension*int;

  var levels:           domain(Level);
  var level_numbers:    [levels] int;
  var top_level:        Level;
  var bottom_level:     Level;
  
  var coarse_interfaces: [levels] LevelInterface;
  var fine_interfaces:   [levels] LevelInterface;

  var boundary_ghosts: [levels] LevelGhostCells;

  def bottom_level_number {
    return level_numbers(bottom_level);
  }

  def ordered_levels {
    for level_number in [1..bottom_level_number] do
      yield levelFromNumber(level_number);
  }


  //===> levelFromNumber method ===>
  //===============================>
  //---------------------------------------------
  // Retrieve a level based on its level number.
  //---------------------------------------------
  def levelFromNumber(level_number: int) {

    for level in levels {
      if level_numbers(level) == level_number then
        return level;
    }

    halt("levelByNumber: Requested invalid level number.");

  }
  //<=== levelFromNumber method <===
  //<===============================


  def initialize() {

    //==== Create the top level ====
    top_level = new Level(x_low = x_low,  x_high = x_high,
                              n_cells       = n_coarsest_cells,
                              n_ghost_cells = n_ghost_cells);
    levels.add(top_level);
    level_numbers(top_level) = 1;
    bottom_level = top_level;

  }

}
// /""""""""""""""""""""""""""|
//<=== AMRHierarchy class <===|
// \__________________________|






//|"""""""""""""""""""""""""""""""""""""""""""\
//|===> AMRHierarchy.addRefinedLevel method ===>
//|___________________________________________/
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
  var old_bottom_level = bottom_level;
  var n_refined_cells  = old_bottom_level.n_cells * ref_ratio;

  var new_bottom_level = new Level(x_low = x_low, x_high = x_high,
                                       n_cells       = n_refined_cells,
                                       n_ghost_cells = n_ghost_cells);

  levels.add(new_bottom_level);
  level_numbers(new_bottom_level) = level_numbers(old_bottom_level) + 1;
  bottom_level = new_bottom_level;


  //==== Build new LevelInterface ====
  //----------------------------------------------------------------------
  // Note that the LevelInterface has some information on grid structure,
  // which will need to be filled in later.
  //----------------------------------------------------------------------
  var interface = new LevelInterface(coarse_level = old_bottom_level,
                                     fine_level   = new_bottom_level,
                                     ref_ratio    = ref_ratio);

  fine_interfaces(old_bottom_level)    = interface;
  coarse_interfaces(new_bottom_level)  = interface;

}
// /"""""""""""""""""""""""""""""""""""""""""""|
//<=== AMRHierarchy.addRefinedLevel method <===|
// \___________________________________________|






//|""""""""""""""""""""""""""""""""""""""""""\
//|===> AMRHierarchy.addGridToLevel method ===>
//|__________________________________________/
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

  if level == top_level {
    i_low  = level.snapToVertex(x_low);
    i_high = level.snapToVertex(x_high);
  }
  else {
    var coarser_level = coarse_interfaces(level).coarse_level;
    var ref_ratio     = coarse_interfaces(level).ref_ratio;
    i_low  = coarser_level.snapToVertex(x_low) * ref_ratio;
    i_high = coarser_level.snapToVertex(x_high) * ref_ratio;
  }


  //==== Create new grid ====
  level.addGrid(i_low, i_high);

}
// /""""""""""""""""""""""""""""""""""""""""""|
//<=== AMRHierarchy.addGridToLevel method <===|
// \__________________________________________|





//|"""""""""""""""""""""""""""""""""""""""""\
//|===> AMRHierarchy.completeLevel method ===>
//|_________________________________________/
//---------------------------------------------------------------------
// After all grids have been added to a level, it should be completed. 
// At this point, it is safe to compute its boundary ghosts, and to
// complete its coarse interface.
//---------------------------------------------------------------------
def AMRHierarchy.completeLevel(level: Level) {

  writeln("Completing level ", level_numbers(level));

  level.complete();

  setBoundaryGhosts(level);

  if level != top_level then
    coarse_interfaces(level).complete();
}
// /"""""""""""""""""""""""""""""""""""""""""|
//<=== AMRHierarchy.completeLevel method <===|
// \_________________________________________|




//"""""""""""""""""""""""""""""""""""""""""""""\
//===> AMRHierarchy.setBoundaryGhosts method ===>
//_____________________________________________/
//-----------------------------------------------------------------
// Sets up boundary_ghosts for a particular level.  The GhostCells
// object for a particular grid is only added if it contains
// nonempty domains.
//-----------------------------------------------------------------
def AMRHierarchy.setBoundaryGhosts(level: Level) {

  boundary_ghosts(level) = new LevelGhostCells(level = level);

  for grid in level.grids {

    var ghost_cells = new GhostCells(grid);
    var has_boundary = false;

    for loc in ghost_locations {

      //==== Check ghost_cells(loc) for physical boundary ====
      //-----------------------------------------------------------------
      // Any single ghost cell domain must either be completely interior
      // or completely exterior.  Hence, it is boundary if and only
      // if its intersection with level.cells is empty.
      //-----------------------------------------------------------------
      if ghost_cells(loc)(level.cells).numIndices == 0 
	&& loc != interior_loc then
	has_boundary = true;
      else
	ghost_cells(loc) = empty_domain;

    }

    //==== Add to the list if there's a boundary intersection ====
    if has_boundary then
      boundary_ghosts(level)(grid) = ghost_cells;

  } // end for grid in level.grids

}
// /"""""""""""""""""""""""""""""""""""""""""""""|
//<=== AMRHierarchy.setBoundaryGhosts method <===|
// \_____________________________________________|




















//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> hierarchyFromInputFile routine ===>
//|______________________________________/
def hierarchyFromInputFile(file_name: string){
 
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
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== hierarchyFromInputFile routine <===|
// \______________________________________|








def main {

  var hierarchy = hierarchyFromInputFile("space.txt");
  var level: Level;

  for i_level in [1..hierarchy.level_numbers(hierarchy.bottom_level)] {
    
    level = hierarchy.levelFromNumber(i_level);
    writeln("Level number ", hierarchy.level_numbers(level), ":");
    writeln("");

    for grid in level.grids {
      writeln("   x_low:   ", grid.x_low);
      writeln("   x_high:  ", grid.x_high);
      writeln("   i_low:   ", grid.i_low);
      writeln("   i_high:  ", grid.i_high);
      writeln("   n_cells: ", grid.n_cells);
      writeln("");
    }

    writeln("");
  }

}