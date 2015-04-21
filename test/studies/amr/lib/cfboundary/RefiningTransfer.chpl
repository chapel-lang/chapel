
use LevelSolution_def;
use CFUtilities;
use MultiArray_def;


//|\""""""""""""""""""""""""""""""""|\
//| >    class: GridCFGhostRegion    | >
//|/________________________________|/
//
//----------------------------------------------------------------
// Describes the portion of the Grid's boundary that is "fine",
// i.e. which overlaps with a coarse grid, but not with a sibling
// on the same level.
//----------------------------------------------------------------

class GridCFGhostRegion {  

  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/

  const grid:             Grid;
  const coarse_neighbors: domain(Grid);
  const transfer_regions:  [coarse_neighbors] MultiDomain(dimension,stridable=true);
  
  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  

  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  
  proc GridCFGhostRegion (
    grid:         Grid,
    parent_level: Level,
    coarse_level: Level)
  {
        
    this.grid = grid;
    
    
    //==== Calculate refinement ratio ====
    const ref_ratio = refinementRatio( coarse_level, parent_level );
    

    for coarse_grid in coarse_level.grids {
      
      var fine_intersection = grid.extended_cells( refine(coarse_grid.cells, ref_ratio) );
      
      if fine_intersection.numIndices > 0 {
        
        //---- Initialize a MultiDomain to the intersection, less grid's interior ----
        
        var boundary_multidomain = new MultiDomain(dimension, stridable=true);
        boundary_multidomain.add( fine_intersection );
        boundary_multidomain.subtract( grid.cells );
        

        //----Remove sibling ghost regions ----
        
        for (neighbor, region) in parent_level.sibling_ghost_regions(grid) {
          
          if fine_intersection(region).numIndices > 0 {
            boundary_multidomain.subtract(region);     
          } 
        }
        

        //---- If boundary_multidomain is still nonempty, update coarse_neighbors and multidomains ----
        
        if !boundary_multidomain.isEmpty()
        {
          coarse_neighbors.add(coarse_grid);
          transfer_regions(coarse_grid) = boundary_multidomain;
        }
        else delete boundary_multidomain;
        
      }
    }

  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|



  //|\'''''''''''''''''''|\
  //| >    destructor    | >
  //|/...................|/
  
  proc ~GridCFGhostRegion ()
  {       
    for multidomain in transfer_regions do delete multidomain;
  }
  // /|'''''''''''''''''''/|
  //< |    destructor    < |
  // \|...................\|
  


  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/
  
  iter these() 
  {
    for neighbor in coarse_neighbors do
      yield ( neighbor, transfer_regions(neighbor) );
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    special method: these    < |
  // \|..............................\|
  

}
// /|""""""""""""""""""""""""""""""""/|
//< |    class: GridCFGhostRegion    < |
// \|________________________________\|






//|\""""""""""""""""""""""""""""""""""|\
//| >    class: LevelCFGhostRegion    | >
//|/__________________________________|/
//
//-------------------------------------------------------------------------
// This object describes the portion of a level's boundary that is "fine",
// i.e. which overlaps with a coarse level.  This is accomplished by a
// collection of GridCFGhostRegion objects.
//-------------------------------------------------------------------------

class LevelCFGhostRegion {

  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/
  
  const level:               Level;  
  const coarse_level:        Level;
  var grid_cf_ghost_regions: [level.grids] GridCFGhostRegion;
  
  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  
  
  
  //|\'''''''''''''''''''''''''''''''''''|\
  //| >    special method: initialize    | >
  //|/...................................|/
  //
  //--------------------------------------------------------------------
  // Intended constructor signature is
  //    LevelCFGhostRegion( level, coarse_level ).
  // The 'initalize' method is required instead of a proper constructor
  // because grid_cf_ghost_regions depends on level.grids.
  //--------------------------------------------------------------------
  
  proc initialize ()
  { 
        
    for grid in level.grids do
      grid_cf_ghost_regions(grid) = new GridCFGhostRegion(grid, level, coarse_level);
      
  }
  // /|'''''''''''''''''''''''''''''''''''/|
  //< |    special method: initialize    < |
  // \|...................................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >    destructor    | >
  //|/...................|/
  
  proc ~LevelCFGhostRegion ()
  {
    for region in grid_cf_ghost_regions do delete region;
  }
  // /|'''''''''''''''''''/|
  //< |    destructor    < |
  // \|...................\|
  
  
  //|\'''''''''''''''''''''''''''''|\
  //| >    special method: this    | >
  //|/.............................|/
  proc this ( grid: Grid )
  {
    return grid_cf_ghost_regions( grid );
  }
  // /|'''''''''''''''''''''''''''''/|
  //< |    special method: this    < |
  // \|.............................\|
  
}
// /|""""""""""""""""""""""""""""""""""/|
//< |    class: LevelCFGhostRegion    < |
// \|__________________________________\|






//|\"""""""""""""""""""""""""""""""""""|\
//| >    class: GridCFGhostSolution    | >
//|/___________________________________|/
//
//-----------------------------------------------------------------------
// This object represents values of a PDE solution on the fine boundary
// of a grid at two time points.  This data will be interpolated to
// provide boundary conditions for the grid in question.
//-----------------------------------------------------------------------

class GridCFGhostSolution {
  
  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/
  
  const grid_cf_ghost_region: GridCFGhostRegion;
  
  var old_data: [grid_cf_ghost_region.coarse_neighbors] MultiArray(dimension,true,real);
  var old_time: real;

  var current_data: [grid_cf_ghost_region.coarse_neighbors] MultiArray(dimension,true,real);
  var current_time: real;


  //---- Not a field, but acts like one ----
  proc grid { return grid_cf_ghost_region.grid; }

  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  
  

  
  
  //|\'''''''''''''''''''''''''''''''''''|\
  //| >    special method: initialize    | >
  //|/...................................|/
  //
  //-----------------------------------------------------------------------------
  // The intended constructor signature is
  //     GridCFGhostSolution( grid_cf_ghost_region: GridCFGhostRegion ).
  // The 'initialize' method is required instead because the fields
  // old_value_multiarrays and current_value_multiarrays are typed by the
  // input.
  //-----------------------------------------------------------------------------
  
  proc initialize ()
  {      
    for c_neighbor in grid_cf_ghost_region.coarse_neighbors {

      old_data(c_neighbor) = new MultiArray(dimension,true,real);
      old_data(c_neighbor).allocate( grid_cf_ghost_region.transfer_regions(c_neighbor) );

      current_data(c_neighbor) = new MultiArray(dimension,true,real);
      current_data(c_neighbor).allocate( grid_cf_ghost_region.transfer_regions(c_neighbor) );
    }
  }
  // /|'''''''''''''''''''''''''''''''''''/|
  //< |    special method: initialize    < |
  // \|...................................\|
  
  
  
  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/
  //
  //------------------------------------------------------------
  // Returns tuples that consist of a fine boundary domain,
  // and the corresponding arrays of old and new boundary data.
  //------------------------------------------------------------
  
  iter these ()
  {  
    for c_neighbor in grid_cf_ghost_region.coarse_neighbors {

      //---- This is a zipper iteration over two objects ----

      for ( old_array,            current_array            ) 
      in  zip( old_data(c_neighbor), current_data(c_neighbor) )
      do
        yield ( old_array, current_array );
    }
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    special method: these    < |
  // \|..............................\|
  
  
  
  //|\'''''''''''''''''''|\
  //| >    destructor    | >
  //|/...................|/
  
  proc ~GridCFGhostSolution () {
    
    for multiarray in old_data do delete multiarray;
    
    for multiarray in current_data do delete multiarray;

  }
  // /|'''''''''''''''''''/|
  //< |    destructor    < |
  // \|...................\|



  //|\"""""""""""""""""""""|\
  //| >    method: fill    | >
  //|/_____________________|/
  //
  //---------------------------------------------------------
  // This method fills the GridCFGhostSolution using refined 
  // data from the input coarse LevelSolution.
  //---------------------------------------------------------

  proc fill ( coarse_level_solution: LevelSolution )
  {

    //---- Calculate refinement ratio ----
    const ref_ratio = refinementRatio( coarse_level_solution.level, this.grid );


    //===> Pull in data from each coarse neighbor grid ===>
    
    for c_neighbor in grid_cf_ghost_region.coarse_neighbors {

      //---- Alias old and current data corresponding to 'c_neighbor' ----
      
      const old_coarse     = coarse_level_solution.old_data( c_neighbor );
      const current_coarse = coarse_level_solution.current_data( c_neighbor );


      //---- Refine data from the coarse grid solution into this structure ----

      for ( old_array,            current_array            )
      in  zip( old_data(c_neighbor), current_data(c_neighbor) )
      {
        var boundary_domain = old_array.domain;
        old_array     = old_coarse.refineValues( boundary_domain, ref_ratio );
        current_array = current_coarse.refineValues( boundary_domain, ref_ratio );
      }

    }
    //<=== Pull in data from each coarse neighbor grid <===


    //---- Copy times ----
    
    this.old_time     = coarse_level_solution.old_time;
    this.current_time = coarse_level_solution.current_time;

  }
  // /|""""""""""""""""""""/|
  //< |   method: fill    < |
  // \|____________________\|

}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    class: GridCFGhostSolution    < |
// \|___________________________________\|






//|\""""""""""""""""""""""""""""""""""""|\
//| >    class: LevelCFGhostSolution    | >
//|/____________________________________|/
//
//-------------------------------------------------------------------
// This object approximates a PDE solution on a LevelFineBoundary,
// at two different time points, 'old_time' and 'current_time'.
// It stores the PDE solution as a collection of GridCFGhostSolution
// objects.
//-------------------------------------------------------------------

class LevelCFGhostSolution {
  
  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/
  
  const level_cf_ghost_region: LevelCFGhostRegion;
  
  const level:                 Level;
  
  var grid_cf_ghost_solutions: [level.grids] GridCFGhostSolution;

  var old_time:     real;    
  var current_time: real;

  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  
  
  
  //|\'''''''''''''''''''''''''''''''''''|\
  //| >    special method: initialize    | >
  //|/...................................|/
  //
  //---------------------------------------------------------------------------
  // This mimics the constructor signature
  //     LevelCFGhostSolution ( level_cf_ghost_region: LevelCFGhostRegion, 
  //                            level: Level ).
  // The 'initialize' method is currently required in place of a proper 
  // constructor because grid_cf_ghost_solutions is typed by level.grids, 
  // which is provided by the input argument.
  //
  // Ideally, the constructor signature would be simply
  //     LevelCFGhostSolution ( level_cf_ghost_region: LevelCFGhostRegion ),
  // but doing so and attempting to set level = level_cf_ghost_region.level
  // creates a nil reference in the meantime, as grid_cf_ghost_solutions
  // attempts to read its domain before this assignment can occur.
  //---------------------------------------------------------------------------
  
  proc initialize ()
  {
    
    
    assert(level == level_cf_ghost_region.level,
           "Error: LevelCFGhostRegion.initialize: Input level must equal level_cf_ghost_region.level");
    
    for grid in level.grids do
      grid_cf_ghost_solutions(grid) = new GridCFGhostSolution( level_cf_ghost_region(grid) );

  }
  // /|'''''''''''''''''''''''''''''''''''/|
  //< |    special method: initialize    < |
  // \|...................................\|
 
 
 
  //|\'''''''''''''''''''|\
  //| >    destructor    | >
  //|/...................|/
  
  proc ~LevelCFGhostSolution () 
  {  
    for solution in grid_cf_ghost_solutions do delete solution;
  }
  // /|'''''''''''''''''''/|
  //< |    destructor    < |
  // \|...................\|
  
  
  
  //|\"""""""""""""""""""""""""""""|\
  //| >    special method: this    | >
  //|/_____________________________|/
  //
  //-----------------------------------------------------------
  // Provided 'grid' in level.grids, returns the corresponding
  // GridCFGhostSolution.
  //-----------------------------------------------------------
  
  proc this ( grid: Grid )
  {
    return grid_cf_ghost_solutions( grid );
  }
  
  // /|"""""""""""""""""""""""""""""/|
  //< |    special method: this    < |
  // \|_____________________________\|
  
  
  
  //|\"""""""""""""""""""""|\
  //| >    method: fill    | >
  //|/_____________________|/
  //
  //---------------------------------------------------------------
  // This method fills the LevelFineBoundarySolution using refined 
  // data from the input coarse LevelSolution.
  //---------------------------------------------------------------
  
  proc fill ( coarse_level_solution: LevelSolution)
  {
    //==== Make aliases for the levels involved ====
    const level        = this.level_cf_ghost_region.level;
    const coarse_level = this.level_cf_ghost_region.coarse_level;

    //==== Safety check ====
    assert(coarse_level == coarse_level_solution.level);

    //==== Prepare ghost data of coarse_level_solution ====
    coarse_level_solution.old_data.extrapolateGhostData();
    coarse_level_solution.old_data.fillOverlaps();

    coarse_level_solution.current_data.extrapolateGhostData();
    coarse_level_solution.current_data.fillOverlaps();


    //==== Fill each GridCFGhostSolution ====
    for grid in level.grids do
      grid_cf_ghost_solutions(grid).fill( coarse_level_solution );


    //==== Copy times ====
    old_time     = coarse_level_solution.old_time;
    current_time = coarse_level_solution.current_time;
  }
  // /|"""""""""""""""""""""/|
  //< |    method: fill    < |
  // \|_____________________\|
  
}
// /|""""""""""""""""""""""""""""""""""""/|
//< |    class: LevelCFGhostSolution    < |
// \|____________________________________\|






//|\"""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    added method: GridVariable.fillCFGhostRegion    | >
//|/_____________________________________________________|/
//
//-----------------------------------------------------------------
// This method fills the fine boundary of the calling GridVariable
// using the provided GridCFGhostSolution.  Data is linearly
// interpolated to time 'time'.
//-----------------------------------------------------------------

proc GridVariable.fillCFGhostRegion (
  grid_cf_ghost_solution: GridCFGhostSolution,
  time:                   real )
{
  //==== Safety check ====
  assert( this.grid == grid_cf_ghost_solution.grid );
  
  
  //==== Pull times from the fine boundary solution ====
  const t1 = grid_cf_ghost_solution.old_time;
  const t2 = grid_cf_ghost_solution.current_time;
  
  
  //==== Make sure that t1 < time < t2, with small margin for roundoff error ====
  assert(time > t1 - 1.0E-8  &&  time < t2 + 1.0E-8,
	 "Warning: LevelVariable.getFineBoundaryValues\n" +
	 "Requesting fine data at time " + format("%8.4Er",time) + "\n" +
	 "coarse_overlap_solution.old_time =     " + format("%8.4Er", t1) + "\n" +
	 "coarse_overlap_solution.current_time = " + format("%8.4Er", t2));
  
  
  //==== Interpolate ====
  const c2 = (time - t1) / (t2 - t1);
  const c1 = 1 - c2;

  for (array1, array2) in grid_cf_ghost_solution 
  {
    var boundary_domain = array1.domain;
    value(boundary_domain) = c1*array1 + c2*array2;
  }
  
}
// /|"""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    added method: GridVariable.fillCFGhostRegion    < |
// \|_____________________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    added method: LevelVariable.fillCFGhostRegion    | >
//|/______________________________________________________|/
//
//------------------------------------------------------------------
// This method fills the fine boundary of the calling LevelVariable
// using the provided LevelFineBoundarySolution.  Data is linearly
// interpolated to time 'time'.  (Details are handled in the
// corresponding method for a GridVariable.)
//------------------------------------------------------------------

proc LevelVariable.fillCFGhostRegion (
  level_cf_ghost_solution: LevelCFGhostSolution,
  time:                    real )
{

  for grid in level.grids do
    this(grid).fillCFGhostRegion( level_cf_ghost_solution(grid), time );
  
}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    added method: LevelVariable.fillCFGhostRegion    < |
// \|______________________________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    added method: GridVariable.refineValues    | >
//|/________________________________________________|/
//
//---------------------------------------------------------------
// Returns values from the GridVariable linearly interpolated to 
// the input domain fine_cells.  This is akin to accessing the
// GridVariable's values on a finer index space.
//---------------------------------------------------------------

proc GridVariable.refineValues (
  fine_cells: domain(dimension,stridable=true),
  ref_ratio:  dimension*int )
{

  var coarse_cells  = grid.cells( coarsen(fine_cells, ref_ratio) );

  var coarse_values = value(coarse_cells);
  var coarse_diffs: [coarse_cells] dimension*real;


  //===> Form interpolant data (values and differentials ===>
  forall cell in coarse_cells {
    var diff_mag, diff_sign, diff_low, diff_high, diff_cen: real;
    var shift: dimension*int;

    for d in dimensions {
      shift *= 0;
      shift(d) = 2;

      diff_low  = value(cell) - value(cell-shift);
      diff_high = value(cell+shift) - value(cell);
      diff_cen  = (diff_high - diff_low) / 2.0;

      if diff_low*diff_high > 0 {
        diff_sign = diff_low / abs(diff_low);
        diff_mag = min( abs(diff_low), abs(diff_high), abs(diff_cen) );
        coarse_diffs(cell)(d) = diff_sign * diff_mag;
      }
      else
        coarse_diffs(cell)(d) = 0.0;
    }
  }
  //<=== Form interpolant data (values and differentials) <===


  //===> Evaluate interpolant on fine cells ===>
  var fine_values: [fine_cells] real;

  forall fine_cell in fine_cells {
    var coarse_cell = coarsen(fine_cell, ref_ratio);
    fine_values(fine_cell) = coarse_values(coarse_cell);

    var fine_displacement: real = 0.0;

    for d in dimensions {
      //==== Move to coarse indices ====
      fine_displacement = fine_cell(d):real / ref_ratio(d):real;

      //==== Compute displacement ====
      fine_displacement -= coarse_cell(d):real;

      //==== Rescale: One cell occupies 2 indices ====
      fine_displacement /= 2.0;

      //==== Modify fine_value ====
      fine_values(fine_cell) += fine_displacement * coarse_diffs(coarse_cell)(d);
    }      

  }
  //<=== Evaluate interpolant on fine cells <===


  return fine_values;

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    added method: GridVariable.refineValues    < |
// \|________________________________________________\|

