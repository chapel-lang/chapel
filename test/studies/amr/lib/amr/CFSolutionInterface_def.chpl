use CFLevelInterface_def;
use LevelSolution_def;
use LevelGhostArraySet_def;


//|"""""""""""""""""""""""""""""""""\
//|===> CFSolutionInterface class ===>
//|_________________________________/
//------------------------------------------------------------------------
// Handles interpolation of fine boundary data from the coarse_solution,
// and correction of the coarse_solution after fine_solution has been
// advanced.
//
// There are three main tasks handled here (they appear in reverse
// order in the code):
//
//    1. Fine-to-coarse spatial interpolation: On the way up an AMR
//       cycle, coarse data is corrected in locations where fine data
//       is available.  No special data is required for this.
//
//    2. Coarse-to-fine spatial interpolation: On the way down an AMR
//       cycle, coarse data is transferred to the locations where it will
//       be needed by the fine level.  The coarse data is available at
//       two timepoints, and the interpolated fine data is stored in
//       old_fine_boundary_data and current_fine_boundary_data.  ("Old"
//       and "current" are in relation to the coarse level.)
//
//    3. Fine boundary interpolation in time: Whenever a fine LevelArray
//       needs boundary data, it will use linear interpolation in time
//       to obtain this from old_fine_boundary_data and 
//       current_fine_boundary_data.
//
// Note that coarse-to-fine interpolation is currently applied on *all*
// fine ghost cells (excluding the physical boundary).  These values
// will later be overwritten on ghost cells that are shared between grids
// on the fine level.  This currently seems to be an acceptable waste,
// compared to the effort it would take to perform this process sharply.
// (Also, in some circumstances, one may want the coarse data in all
// locations, e.g. for a domain decomposition.)
//
//------------------------------------------------------------------------
class CFSolutionInterface {

  const coarse_solution:      LevelSolution;
  const fine_solution:        LevelSolution;
  const level_interface:      CFLevelInterface;

  var old_fine_boundary_data:     LevelGhostArraySet;
  var current_fine_boundary_data: LevelGhostArraySet;
  var old_time:     real;
  var current_time: real;


  //|:::::::::::::::::::\
  //|===> Constructor ===>
  //|:::::::::::::::::::/
  def CFSolutionInterface(
    coarse_solution: LevelSolution,
    fine_solution:   LevelSolution,
    level_interface: CFLevelInterface) 
  {
    this.coarse_solution = coarse_solution;
    this.fine_solution   = fine_solution;
    this.level_interface = level_interface;

    var fine_level = fine_solution.level;

    old_fine_boundary_data     = new LevelGhostArraySet(fine_solution.level);
    current_fine_boundary_data = new LevelGhostArraySet(fine_solution.level);

  }
  // /:::::::::::::::::::/
  //<=== Constructor <==<
  // \:::::::::::::::::::\


  //|::::::::::::::::::::::::::::::::::::::::::::\
  //|===> Coarse-to-fine spatial interpolation ===>
  //|::::::::::::::::::::::::::::::::::::::::::::/
  def fillFineBoundaryData_Linear() {
    old_fine_boundary_data.interpolateFromCoarse_Linear(coarse_solution.old_data, level_interface);
    old_time = coarse_solution.old_time;
    
    current_fine_boundary_data.interpolateFromCoarse_Linear(coarse_solution.current_data, level_interface);
    current_time = coarse_solution.current_time;
  }
  // /::::::::::::::::::::::::::::::::::::::::::::/
  //<=== Coarse-to-fine spatial interpolation ===<
  // \::::::::::::::::::::::::::::::::::::::::::::\


  //|::::::::::::::::::::::::::::::::::::::::::::\
  //|===> Fine-to-coarse spatial interpolation ===>
  //|::::::::::::::::::::::::::::::::::::::::::::/
  def correctCoarseSolution_Linear()
  {
    assert( abs(coarse_solution.current_time - fine_solution.current_time) < 1.0e-8,
	    "Warning: correctCoarseInterior_Linear\n" +
	    "  Time of fine solution may not agree with time of coarse solution");

    coarse_solution.current_data.interpolateFromFine_Linear(fine_solution.current_data, 
                                                            level_interface);
  }
  // /::::::::::::::::::::::::::::::::::::::::::::/
  //<=== Fine-to-coarse spatial interpolation ===<
  // \::::::::::::::::::::::::::::::::::::::::::::\

}
// /"""""""""""""""""""""""""""""""""/
//<=== CFSolutionInterface class <==<
// \_________________________________\





//|""""""""""""""""""""""""""""""""""""""""""""""""\
//|===> LevelArray.fineBoundaryTimeInterpolation ===>
//|________________________________________________/
//--------------------------------------------------------------------
// Interpolates ghost cell data via linear interpolation in time from 
// old_fine_boundary_data and current_fine_boundary_data.
//--------------------------------------------------------------------
def LevelArray.fineBoundaryTimeInterpolation(
  sol_interface: CFSolutionInterface,
  t:             real)
{

  var t1 = sol_interface.old_time;
  var t2 = sol_interface.current_time;

  //==== Safety check the requested time ====
  assert(t >= t1-1.0E-8 && t <= t2+1.0E-8,
	 "Warning: LevelSolution.interpolateCFBoundary\n" +
	 "Requesting fine data at time " + format("%8.4E",t) + "\n" +
	 "CFSolutionInterface.old_time =     " + format("%8.4E",sol_interface.old_time) + "\n" +
	 "CFSolutionInterface.current_time = " + format("%8.4E",sol_interface.current_time));


  //==== Interpolation parameters ====
  var a2 = (t - t1) / (t2 - t1);
  var a1 = 1 - a2;


  //===> Interpolate! ===>
  for grid in level.grids {

    var old_ghost_arrays     = sol_interface.old_fine_boundary_data(grid);
    var current_ghost_arrays = sol_interface.current_fine_boundary_data(grid);

    for (old_array,current_array) in (old_ghost_arrays,current_ghost_arrays) {
      var ghost_domain = old_array.dom;
      assert(old_array.dom == current_array.dom);

      this(grid,ghost_domain) = a1 * old_array.value + a2 * current_array.value;
    }

/*     for loc in ghost_locations { */
      
/*       var ghost_domain = grid.ghost_domain_set(loc); */

/*       this(grid,ghost_domain) = a1 * old_ghost_array_set(loc).value  */
/*                                   + a2 * current_ghost_array_set(loc).value; */
/*     } */
  }
  //<=== Interpolate! <===

}
// /""""""""""""""""""""""""""""""""""""""""""""""""/
//<=== LevelArray.fineBoundaryTimeInterpolation <==<
// \________________________________________________\






//|""""""""""""""""""""""""""""""""""""""""""""\
//|===> Coarse-to-fine spatial interpolation ===>
//|____________________________________________/

//|'''''''''''''''''''''''''''''''''''''''''''''''''''''''\
//|===> LevelGhostArraySet.interpolateFromCoarse_Linear ===>
//|......................................................./
def LevelGhostArraySet.interpolateFromCoarse_Linear(
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{

  //==== Prepare ghost data of q_coarse ====
  q_coarse.extrapolateGhostData();
  q_coarse.fillSharedGhosts();

  //==== Interpolate on each fine grid ====
  for grid in level.grids do
    ghost_array_sets(grid).interpolateFromCoarse_Linear(q_coarse, interface);

}
// /:::::::::::::::::::::::::::::::::::::::::::::::::::::::/
//<=== LevelGhostArraySet.interpolateFromCoarse_Linear <==<
// \:::::::::::::::::::::::::::::::::::::::::::::::::::::::\



//|:::::::::::::::::::::::::::::::::::::::::::::::::::::::::\
//|===> GhostArraySet.interpolateFromCoarse_Linear method ===>
//|:::::::::::::::::::::::::::::::::::::::::::::::::::::::::/
def GhostArraySet.interpolateFromCoarse_Linear(
  q_coarse:  LevelArray,
  interface: CFLevelInterface)
{
  var overlap: domain(dimension, stridable=true);

  for coarse_grid in interface.coarse_neighbors(this.grid) {

    for ghost_array in array_set {
      var ghost_domain = ghost_array.dom;
      overlap = ghost_domain( interface.refine(coarse_grid.cells) );
      
      if overlap.numIndices > 0 then
	ghost_array.value(overlap) =
	  q_coarse(coarse_grid).interpolateToFine_Linear(overlap, interface);
    } // end for ghost_array in array_set 

/*     for loc in ghost_locations { */
/*       if loc != interior_location { */
/*         overlap = grid.ghost_domain_set(loc)( interface.refine(coarse_grid.cells) ); */

/*         if overlap.numIndices > 0 then */
/*           arrays(loc).value(overlap) =  */
/*             q_coarse(coarse_grid).interpolateToFine_Linear(overlap, interface); */
/*       } */
/*     } */


  } // end for coarse_grid in interface.coarse_neighbors(this.grid)

}
// /:::::::::::::::::::::::::::::::::::::::::::::::::::::::::/
//<=== GhostArraySet.interpolateFromCoarse_Linear method <==<
// \:::::::::::::::::::::::::::::::::::::::::::::::::::::::::\



//|:::::::::::::::::::::::::::::::::::::::::::::::::\
//|===> GridArray.interpolateToFine_Linear method ===>
//|:::::::::::::::::::::::::::::::::::::::::::::::::/
def GridArray.interpolateToFine_Linear(
  fine_cells: domain(dimension, stridable=true),
  interface:  CFLevelInterface)
{
  
  var coarse_cells  = grid.cells( interface.coarsen(fine_cells) );
  
  var coarse_values = value(coarse_cells);
  var coarse_diffs: [coarse_cells] [dimensions] real;


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
    var coarse_cell = interface.coarsen(fine_cell);
    fine_values(fine_cell) = coarse_values(coarse_cell);

    var fine_displacement: real = 0.0;

    for d in dimensions {
      //==== Move to coarse indices ====
      fine_displacement = fine_cell(d):real / interface.ref_ratio(d):real;

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
// /:::::::::::::::::::::::::::::::::::::::::::::::::/
//<=== GridArray.interpolateToFine_Linear method <==<
// \:::::::::::::::::::::::::::::::::::::::::::::::::\



//|::::::::::::::::::::::::::::::::::::\
//|===> extrapolateGhostData methods ===>
//|::::::::::::::::::::::::::::::::::::/
//-----------------------------------------------------------------
// Fills the first layer of ghost cells with linearly extrapolated
// data from the interior.
//-----------------------------------------------------------------
def LevelArray.extrapolateGhostData() {
  
  for grid_array in grid_arrays do
    grid_array.extrapolateGhostData();

}


def GridArray.extrapolateGhostData() {

  for ghost_domain in grid.ghost_domain_set {
    var loc = grid.relativeLocation(ghost_domain);
    var shift = -1*loc;

    forall cell in ghost_domain do
      value(cell) = 2.0*value(cell+shift) - value(cell+2*shift);
  }

/*   var shift: dimension*int; */

/*   for loc in ghost_locations { */
/*     //==== Create index pointing towards interior ==== */
/*     shift = -2*loc; */
    
/*     forall cell in grid.ghost_domain_set(loc) do */
/*       value(cell) = 2.0*value(cell+shift) - value(cell+2*shift);     */
/*   } */

}
// /::::::::::::::::::::::::::::::::::::/
//<=== extrapolateGhostData methods <==<
// \::::::::::::::::::::::::::::::::::::\

// /""""""""""""""""""""""""""""""""""""""""""""/
//<=== Coarse-to-fine spatial interpolation <==<
// \____________________________________________\







//|"""""""""""""""""""""""""""""""""\
//|===> Fine-to-coarse correction ===>
//|_________________________________/

//|:::::::::::::::::::::::::::::::::::::::::::::\
//|===> LevelArray.interpolateFromFine_Linear ===>
//|:::::::::::::::::::::::::::::::::::::::::::::/
def LevelArray.interpolateFromFine_Linear(
  q_fine:    LevelArray,
  interface: CFLevelInterface)
{

  for grid_array in grid_arrays do
    grid_array.interpolateFromFine_Linear(q_fine, interface);

}
// /:::::::::::::::::::::::::::::::::::::::::::::/
//<=== LevelArray.interpolateFromFine_Linear <==<
// \:::::::::::::::::::::::::::::::::::::::::::::\



//|::::::::::::::::::::::::::::::::::::::::::::\
//|===> GridArray.interpolateFromFine_Linear ===>
//|::::::::::::::::::::::::::::::::::::::::::::/
def GridArray.interpolateFromFine_Linear(
  q:         LevelArray,
  interface: CFLevelInterface)
{

  var overlap: domain(dimension, stridable=true);

  for fine_grid in interface.fine_neighbors(this.grid) {
    overlap = this.grid.cells( interface.coarsen(fine_grid.cells) );
    // Transfer data between processors here
    value(overlap) = q(fine_grid).interpolateToCoarse_Linear(overlap, interface);
  }

}
// /::::::::::::::::::::::::::::::::::::::::::::/
//<=== GridArray.interpolateFromFine_Linear <==<
// \::::::::::::::::::::::::::::::::::::::::::::\



//|::::::::::::::::::::::::::::::::::::::::::::\
//|===> GridArray.interpolateToCoarse_Linear ===>
//|::::::::::::::::::::::::::::::::::::::::::::/
def GridArray.interpolateToCoarse_Linear(
  coarse_cells: domain(dimension, stridable=true),
  interface:    CFLevelInterface)
{

  //==== Volume fraction is 1/product(ref_ratio) ====
  // Could store this in the interface...
  var volume_fraction: real = 1.0;
  for d in dimensions do
    volume_fraction /= interface.ref_ratio(d):real;

  
  //==== Compute coarse averages ====
  var coarse_values: [coarse_cells] real;

  forall coarse_cell in coarse_cells {
    var fine_cells = interface.refine(coarse_cell);
    for fine_cell in fine_cells do
      coarse_values(coarse_cell) += value(fine_cell);
    coarse_values(coarse_cell) *= volume_fraction;
  }


  return coarse_values;

}
// /::::::::::::::::::::::::::::::::::::::::::::/
//<=== GridArray.interpolateToCoarse_Linear <==<
// \::::::::::::::::::::::::::::::::::::::::::::\

// /"""""""""""""""""""""""""""""""""/
//<=== Fine-to-coarse correction <==<
// \_________________________________\