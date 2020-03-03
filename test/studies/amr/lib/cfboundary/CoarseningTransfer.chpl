
public use LevelSolution_def;
public use CFUtilities;



//|\"""""""""""""""""""""""""""""""""|\
//| >    class: GridInvalidRegion    | >
//|/_________________________________|/

//---------------------------------------------------------------------
// Stores the region on which a coarse grid will receive data from a
// finer level.  This consists of grid.cells, intersected with the
// coarsening of fine_grid.cells.
//---------------------------------------------------------------------

class GridInvalidRegion {
  
  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/
  
  const fine_neighbors: domain(unmanaged Grid);
  const domains:        [fine_neighbors] domain(dimension,stridable=true);
  
  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  
    
  
  //|\''''''''''''''''''''|\
  //| >    initializer    | >
  //|/....................|/
  
  proc init (
    grid:         unmanaged Grid,
    parent_level: unmanaged Level,
    fine_level:   unmanaged Level )
  {
    this.complete();
    //==== Calculate refinement ratio ====
    const ref_ratio = refinementRatio(parent_level, fine_level);
    
    //==== Check each grid on fine_level for overlap ====
    for fine_grid in fine_level.grids 
    {
      var coarse_intersection = grid.cells( coarsen(fine_grid.cells, ref_ratio) );
      
      if coarse_intersection.numIndices>0 {
        fine_neighbors.add(fine_grid);
        domains(fine_grid) = coarse_intersection;
      }
    }
  }
  // /|''''''''''''''''''''/|
  //< |    initializer    < |
  // \|....................\|
  
  
  
  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/

  iter these () 
  {
    for f_neighbor in fine_neighbors do
      yield (f_neighbor, domains(f_neighbor));
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    special method: these    < |
  // \|..............................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit () {}

  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|
  
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    class: GridInvalidRegion    < |
// \|_________________________________\|






//|\""""""""""""""""""""""""""""""""""|\
//| >    class: LevelInvalidRegion    | >
//|/__________________________________|/

class LevelInvalidRegion {
  
  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/
  
  const level:      unmanaged Level;
  const fine_level: unmanaged Level;
  
  var grid_invalid_regions: [level.grids] unmanaged GridInvalidRegion?;

  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  


  //|\'''''''''''''''''''''''''''''''''''|\
  //| >    special method: initialize    | >
  //|/...................................|/

  //----------------------------------------------------------------------
  // The intended constructor signature is
  //     LevelInvalidRegion ( level: Level, fine_level: Level ).
  // The 'initialize' method is currently required because the field
  // grid_invalid_regions is typed by the input 'level'.
  //----------------------------------------------------------------------

  proc postinit ()
  {
    for grid in level.grids do
      grid_invalid_regions(grid) = new unmanaged GridInvalidRegion(grid, level, fine_level);
  }
  // /|'''''''''''''''''''''''''''''''''''/|
  //< |    special method: initialize    < |
  // \|...................................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/
  
  proc deinit ()
  {
    for region in grid_invalid_regions do delete region;
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|
  


  //|\'''''''''''''''''''''''''''''|\
  //| >    special method: this    | >
  //|/.............................|/
  
  proc this (grid: unmanaged Grid) 
  {
    return grid_invalid_regions(grid);
  }
  // /|'''''''''''''''''''''''''''''/|
  //< |    special method: this    < |
  // \|.............................\|
  
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    class: LevelInvalidRegion    < |
// \|__________________________________\|






//|\"""""""""""""""""""""""""""""""""""""""""""|\
//| >    LevelSolution.correctInvalidRegion    | >
//|/___________________________________________|/

//----------------------------------------------------------------------
// This method corrects the calling LevelSolution on its invalid region
// (described by input 'level_invalid_region'), using data coarsened
// from 'fine_level_solution'.
//----------------------------------------------------------------------

proc LevelSolution.correctInvalidRegion (
  level_invalid_region: unmanaged LevelInvalidRegion,
  fine_level_solution:  unmanaged LevelSolution )
{
  //==== Safety checks ====
  assert(this.level == level_invalid_region.level);
  assert(fine_level_solution.level == level_invalid_region.fine_level);
  assert( abs(this.current_time - fine_level_solution.current_time) < 1.0e-8);
  
  //==== Correct ====
  current_data.fillInvalidRegion( level_invalid_region, fine_level_solution.current_data );
}
// /|"""""""""""""""""""""""""""""""""""""""""""/|
//< |    LevelSolution.correctInvalidRegion    < |
// \|___________________________________________\|






//|\"""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    added method: LevelVariable.fillInvalidRegion   | >
//|/_____________________________________________________|/

//------------------------------------------------------------
// This method fills the LevelVariable on its invalid region,
// obtained by coarsening the data in fine_level_variable.
//------------------------------------------------------------

proc LevelVariable.fillInvalidRegion (
  level_invalid_region: unmanaged LevelInvalidRegion,
  fine_level_variable:  unmanaged LevelVariable )
{

  //---- Safety check ----
  assert(this.level                == level_invalid_region.level);
  assert(fine_level_variable.level == level_invalid_region.fine_level);
  
  //---- Refinement ratio ----
  const ref_ratio = refinementRatio(this.level, fine_level_variable.level);

  //---- Each grid obtains coarsened values from fine neighbors ----
  for grid in this.level.grids {
    for (f_neighbor,Domain) in level_invalid_region(grid)! do
      this(grid,Domain) = fine_level_variable(f_neighbor)!.coarsenValues(Domain,ref_ratio);
  }
}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    added method: LevelVariable.fillInvalidRegion    < |
// \|______________________________________________________\|





//|\"""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    added method: GridVariable.coarsenValues    | >
//|/_________________________________________________|/

//-------------------------------------------------------------------
// Returns values from the GridVariable linearly interpolated to the 
// input domain 'coarse_cells'.  This is akin to accessing the
// GridVariable's values on a coarser index space.
//-------------------------------------------------------------------

proc GridVariable.coarsenValues (
  coarse_cells: domain(dimension,stridable=true),
  ref_ratio:    dimension*int)
{

  //---- Volume fraction is 1/product(ref_ratio) ----
  var volume_fraction: real = 1.0;
  for d in dimensions do
    volume_fraction /= ref_ratio(d):real;


  //---- Compute coarse averages ----
  var coarse_values: [coarse_cells] real;

  forall coarse_cell in coarse_cells {
    var fine_cells = refine(coarse_cell, ref_ratio);
    for fine_cell in fine_cells do
      coarse_values(coarse_cell) += value(fine_cell);
    coarse_values(coarse_cell) *= volume_fraction;
  }

  return coarse_values;

}
// /|"""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    added method: GridVariable.coarsenValues    < |
// \|_________________________________________________\|



