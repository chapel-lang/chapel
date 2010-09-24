use LevelSolution_def;
use CFLevelInterface_def;
use CoarseToFineInterpolation;
use FineToCoarseInterpolation;


//|\""""""""""""""""""""""""""""""""""|\
//| >    CFSolutionInterface class    | >
//|/__________________________________|/
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
//------------------------------------------------------------------------
class CFSolutionInterface {

  const coarse_solution:      LevelSolution;
  const fine_solution:        LevelSolution;
  const level_interface:      CFLevelInterface;

  var old_fine_boundary_data:     LevelGhostArraySet;
  var current_fine_boundary_data: LevelGhostArraySet;
  var old_time:     real;
  var current_time: real;


  //|\''''''''''''''''''''|\
  //| >    Constructor    | >
  //|/....................|/
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
  // /|''''''''''''''''''''/|
  //< |    Constructor    < |
  // \|....................\|


  //|\'''''''''''''''''''''''''''''''''''''''''''''|\
  //| >    Coarse-to-fine spatial interpolation    | >
  //|/.............................................|/
  def getFineBoundaryData_Linear() {
    old_fine_boundary_data.interpolateFromCoarse_Linear(coarse_solution.old_data, level_interface);
    old_time = coarse_solution.old_time;
    
    current_fine_boundary_data.interpolateFromCoarse_Linear(coarse_solution.current_data, level_interface);
    current_time = coarse_solution.current_time;
  }
  // /|'''''''''''''''''''''''''''''''''''''''''''''/|
  //< |    Coarse-to-fine spatial interpolation    < |
  // \|.............................................\|


  //|\'''''''''''''''''''''''''''''''''''''''''''''|\
  //| >    Fine-to-coarse spatial interpolation    | >
  //|/.............................................|/
  def correctCoarseSolution_Linear()
  {
    assert( abs(coarse_solution.current_time - fine_solution.current_time) < 1.0e-8,
	    "Warning: correctCoarseInterior_Linear\n" +
	    "  Time of fine solution may not agree with time of coarse solution");

    coarse_solution.current_data.interpolateFromFine_Linear(fine_solution.current_data, 
                                                            level_interface);
  }
  // /|'''''''''''''''''''''''''''''''''''''''''''''/|
  //< |    Fine-to-coarse spatial interpolation    < |
  // \|.............................................\|

}
// /|""""""""""""""""""""""""""""""""""/|
//< |    CFSolutionInterface class    < |
// \|__________________________________\|






//|\""""""""""""""""""""""""""""""""""""""""""""""""""""|\
//| >    CFSolutionInterface.provideFineBoundaryData    | >
//|/____________________________________________________|/
def CFSolutionInterface.provideFineBoundaryData(
  q: LevelArray,
  t: real)
{

  //==== Safety check on the level ====
  assert(q.level == level_interface.fine_level);

  //==== Safety check the requested time ====
  assert(t > old_time-1.0E-8 && t < current_time+1.0E-8,
	 "Warning: CFSolutionInterface.fillLevelArrayBoundary\n" +
	 "Requesting fine data at time " + format("%8.4E",t) + "\n" +
	 "old_time =     " + format("%8.4E", old_time) + "\n" +
	 "current_time = " + format("%8.4E", current_time));

  //==== Provide data for each grid array ====
  for grid in q.level.grids do
    provideFineBoundaryData(q(grid), t);

}


def CFSolutionInterface.provideFineBoundaryData(
  q: GridArray,
  t: real)
{

  //==== Interpolation parameters ====
  var a2 = (t - old_time) / (current_time - old_time);
  var a1 = 1 - a2;


  //==== Interpolate! ====
  for (old_ghost_array, current_ghost_array) 
    in (old_fine_boundary_data(q.grid), current_fine_boundary_data(q.grid)) 
  {
    var ghost_domain = old_ghost_array.dom;
    assert(current_ghost_array.dom == ghost_domain);

    q.value(ghost_domain) = a1 * old_ghost_array.value 
                            + a2 * current_ghost_array.value;
  }

}
// /|""""""""""""""""""""""""""""""""""""""""""""""""""""/|
//< |    CFSolutionInterface.provideFineBoundaryData    < |
// \|____________________________________________________\|








/* //|\"""""""""""""""""""""""""""""""""""""""""""""""""|\ */
/* //| >    LevelArray.fineBoundaryTimeInterpolation    | > */
/* //|/_________________________________________________|/ */
/* //-------------------------------------------------------------------- */
/* // Interpolates ghost cell data via linear interpolation in time from  */
/* // old_fine_boundary_data and current_fine_boundary_data. */
/* //-------------------------------------------------------------------- */
/* def LevelArray.fineBoundaryTimeInterpolation( */
/*   sol_interface: CFSolutionInterface, */
/*   t:             real) */
/* { */

/*   var t1 = sol_interface.old_time; */
/*   var t2 = sol_interface.current_time; */

/*   //==== Safety check the requested time ==== */
/*   assert(t >= t1-1.0E-8 && t <= t2+1.0E-8, */
/* 	 "Warning: LevelSolution.interpolateCFBoundary\n" + */
/* 	 "Requesting fine data at time " + format("%8.4E",t) + "\n" + */
/* 	 "CFSolutionInterface.old_time =     " + format("%8.4E",sol_interface.old_time) + "\n" + */
/* 	 "CFSolutionInterface.current_time = " + format("%8.4E",sol_interface.current_time)); */


/*   //==== Interpolation parameters ==== */
/*   var a2 = (t - t1) / (t2 - t1); */
/*   var a1 = 1 - a2; */


/*   //===> Interpolate! ===> */
/*   for grid in level.grids { */

/*     var old_ghost_arrays     = sol_interface.old_fine_boundary_data(grid); */
/*     var current_ghost_arrays = sol_interface.current_fine_boundary_data(grid); */

/*     for (old_array,current_array) in (old_ghost_arrays,current_ghost_arrays) { */
/*       var ghost_domain = old_array.dom; */
/*       assert(old_array.dom == current_array.dom); */

/*       this(grid,ghost_domain) = a1 * old_array.value + a2 * current_array.value; */
/*     } */

/*   } */
/*   //<=== Interpolate! <=== */

/* } */
/* // /|"""""""""""""""""""""""""""""""""""""""""""""""""/| */
/* //< |    LevelArray.fineBoundaryTimeInterpolation    < | */
/* // \|_________________________________________________\| */